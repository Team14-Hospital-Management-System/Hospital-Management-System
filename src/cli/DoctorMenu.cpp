#include "hms/Cli.h"
#include <string>
#include <istream>
#include <ostream>
#include "hms/AppContext.h"
namespace hms {
void runDoctorMenu(AppContext&c,std::istream&in,std::ostream&o){
 while(true){o<<"\nDOCTORS\n1 List 2 Add 3 Update 4 Set availability 0 Back\n> ";std::string x;if(!std::getline(in,x)||x=="0")return;
 if(x=="1"){for(auto&d:c.doctors.all())o<<d.id<<" | Dr. "<<d.name<<" | "<<d.specialization<<" | "<<(d.available?"Available":"Unavailable")<<" | "<<d.workStart<<"-"<<d.workEnd<<"\n";}
 else if(x=="2"){Doctor d;o<<"Name: ";std::getline(in,d.name);o<<"Specialization: ";std::getline(in,d.specialization);o<<"Work start HH:MM [09:00]: ";std::string v;std::getline(in,v);if(!v.empty())d.workStart=v;o<<"Work end HH:MM [17:00]: ";std::getline(in,v);if(!v.empty())d.workEnd=v;auto r=c.doctors.addDoctor(d);if(r.ok())o<<"Added doctor ID "<<r.value.id<<"\n";else o<<r.status.message<<"\n";}
 else if(x=="3"){o<<"Doctor ID: ";std::string s;std::getline(in,s);try{auto d=c.doctors.findById(std::stoi(s));if(!d){o<<"Not found.\n";continue;}o<<"Name ["<<d->name<<"]: ";std::string v;std::getline(in,v);if(!v.empty())d->name=v;o<<"Specialization ["<<d->specialization<<"]: ";std::getline(in,v);if(!v.empty())d->specialization=v;o<<"Start ["<<d->workStart<<"]: ";std::getline(in,v);if(!v.empty())d->workStart=v;o<<"End ["<<d->workEnd<<"]: ";std::getline(in,v);if(!v.empty())d->workEnd=v;auto r=c.doctors.updateDoctor(*d);o<<(r?"Updated.\n":r.message+"\n");}catch(...){o<<"Invalid ID.\n";}}
 else if(x=="4"){o<<"Doctor ID: ";std::string s,v;std::getline(in,s);o<<"Available? (1/0): ";std::getline(in,v);try{auto r=c.doctors.setAvailability(std::stoi(s),v=="1");o<<(r?"Updated.\n":r.message+"\n");}catch(...){o<<"Invalid input.\n";}}
 else o<<"Invalid choice.\n";
 }
}
}
