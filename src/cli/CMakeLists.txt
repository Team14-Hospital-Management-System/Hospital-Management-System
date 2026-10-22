add_library(hms_cli
  Cli.cpp PatientMenu.cpp DoctorMenu.cpp AppointmentMenu.cpp
  BillingMenu.cpp ReportMenu.cpp AdminMenu.cpp)
target_link_libraries(hms_cli PUBLIC
  hms_common hms_auth hms_patient hms_doctor hms_appointment
  hms_billing hms_report hms_storage hms_security)
