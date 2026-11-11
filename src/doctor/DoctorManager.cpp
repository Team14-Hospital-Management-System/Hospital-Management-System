#include "hms/DoctorManager.h"
#include <algorithm>
namespace hms {
namespace {bool validTime(const std::string&t){return t.size()==5&&t[2]==':'&&t.substr(0,2).find_first_not_of("0123456789")==std::string::npos&&t.substr(3,2).find_first_not_of("0123456789")==std::string::npos&&t>="00:00"&&t<="23:59";}}
Result<Doctor> DoctorManager::addDoctor(const Doctor&d){
 if(d.name.empty()||d.specialization.empty()||!validTime(d.workStart)||!validTime(d.workEnd)||d.workStart>=d.workEnd)return Result<Doctor>::failure("Invalid doctor details or working hours.");
 Doctor x=d;x.id=nextId_++;doctors_.push_back(x);return Result<Doctor>::success(x);
}
Status DoctorManager::updateDoctor(const Doctor&d){
 auto i=std::find_if(doctors_.begin(),doctors_.end(),[&](const Doctor&x){return x.id==d.id;});if(i==doctors_.end())return Status::failure("Doctor not found.");
 if(d.name.empty()||d.specialization.empty()||!validTime(d.workStart)||!validTime(d.workEnd)||d.workStart>=d.workEnd) return Status::failure("Invalid doctor details.");
 *i=d;
 return Status::success();
}
Status DoctorManager::setAvailability(Id id,bool a){auto i=std::find_if(doctors_.begin(),doctors_.end(),[&](const Doctor&d){return d.id==id;});if(i==doctors_.end())return Status::failure("Doctor not found.");i->available=a;return Status::success();}
bool DoctorManager::exists(Id id)const{return std::any_of(doctors_.begin(),doctors_.end(),[&](const Doctor&d){return d.id==id;});}
std::optional<Doctor> DoctorManager::findById(Id id)const{auto i=std::find_if(doctors_.begin(),doctors_.end(),[&](const Doctor&d){return d.id==id;});if(i==doctors_.end())return {};return *i;}
std::vector<Doctor> DoctorManager::findBySpecialization(const std::string&q)const{std::vector<Doctor>r;for(auto&d:doctors_)if(d.specialization.find(q)!=std::string::npos)r.push_back(d);return r;}
bool DoctorManager::worksAt(Id id,const TimeSlot&s)const{auto d=findById(id);return d&&d->available&&s.date.size()==10&&s.time.size()==5&&s.date[4]=='-'&&s.date[7]=='-'&&s.time>="00:00"&&s.time<="23:59"&&s.time>=d->workStart&&s.time<d->workEnd;}
void DoctorManager::replaceAll(std::vector<Doctor>v){doctors_=std::move(v);nextId_=1;for(auto&d:doctors_)nextId_=std::max(nextId_,d.id+1);}
}
