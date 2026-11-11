#include "hms/ReportManager.h"
#include <iomanip>
#include <sstream>
#include "hms/AppointmentManager.h"
#include "hms/Billing.h"
#include "hms/DoctorManager.h"
#include "hms/PatientManager.h"
namespace hms {
 ReportManager::ReportManager(const PatientManager&p,const DoctorManager&d,const AppointmentManager&a,const Billing&b):patients_(p),doctors_(d),appointments_(a),billing_(b){}
 namespace {std::string money(long long c){std::ostringstream o;o<<"Rs "<<c/100<<"."<<std::setw(2)<<std::setfill('0')<<c%100;return o.str();}}
 Result<std::string> ReportManager::patientReport(Id id)const{
  auto p=patients_.findById(id);if(!p)return Result<std::string>::failure("Patient not found.");
  std::ostringstream o;o<<"PATIENT REPORT\nID: "<<p->id<<"\nName: "<<p->name<<"\nAge: "<<p->age<<"\nPhone: "<<p->phone<<"\nAddress: "<<p->address<<"\nMedical notes: "<<p->medicalNotes<<"\nAppointments:\n";
  for(auto&a:appointments_.forPatient(id)){auto d=doctors_.findById(a.doctorId);o<<"#"<<a.id<<" "<<a.slot.date<<" "<<a.slot.time<<" Dr. "<<(d?d->name:"Unknown")<<" ["<<toString(a.status)<<"]\n";}
  o<<"Outstanding: "<<money(billing_.outstandingCents(id))<<"\n";return Result<std::string>::success(o.str());
  }
 std::string ReportManager::appointmentReport(const std::string&date)const{
  std::ostringstream o;o<<"APPOINTMENTS ON "<<date<<"\n";
  for(auto&a:appointments_.onDate(date)){auto p=patients_.findById(a.patientId);auto d=doctors_.findById(a.doctorId);o<<"#"<<a.id<<" "<<a.slot.time<<" Patient: "<<(p?p->name:"Unknown")<<" Doctor: "<<(d?d->name:"Unknown")<<" Status: "<<toString(a.status)<<"\n";}
  return o.str();
  }
 Result<std::string> ReportManager::doctorScheduleReport(Id id)const{
  auto d=doctors_.findById(id);if(!d)return Result<std::string>::failure("Doctor not found.");
  std::ostringstream o;o<<"SCHEDULE: Dr. "<<d->name<<"\n";for(auto&a:appointments_.forDoctor(id)){auto p=patients_.findById(a.patientId);o<<a.slot.date<<" "<<a.slot.time<<" "<<(p?p->name:"Unknown")<<" ["<<toString(a.status)<<"]\n";}return Result<std::string>::success(o.str());
  }
}
