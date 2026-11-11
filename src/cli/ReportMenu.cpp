#include "hms/Cli.h"
#include <string>
#include <istream>
#include <ostream>
#include "hms/AppContext.h"
namespace hms {
void runReportMenu(AppContext&c,std::istream&in,std::ostream&o){
 while(true){o<<"\nREPORTS\n1 Patient report 2 Appointment report by date 3 Doctor schedule report 0 Back\n> ";std::string x;if(!std::getline(in,x)||x=="0")return;
 if(x=="1"){o<<"Patient ID: ";std::getline(in,x);try{auto r=c.reports.patientReport(std::stoi(x));o<<(r.ok()?r.value:r.status.message+"\n");}catch(...){o<<"Invalid ID.\n";}}
 else if(x=="2"){o<<"Date YYYY-MM-DD: ";std::getline(in,x);o<<c.reports.appointmentReport(x);}
 else if(x=="3"){o<<"Doctor ID: ";std::getline(in,x);try{auto r=c.reports.doctorScheduleReport(std::stoi(x));o<<(r.ok()?r.value:r.status.message+"\n");}catch(...){o<<"Invalid ID.\n";}}
 else o<<"Invalid choice.\n";
 }
}
}
