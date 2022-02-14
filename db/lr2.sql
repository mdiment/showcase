CREATE DATABASE lr2;  

\c lr2;  

CREATE TABLE regions(  

region_id integer PRIMARY KEY,  

budget integer,  

region_name text NOT NULL); 

 

CREATE TABLE treatments(  

treatment_id integer PRIMARY KEY,  

begin_date date,  

end_date date, 

diagnosis text,  

status text); 

 

CREATE TABLE hospitals(  

hospital_id integer PRIMARY KEY,  

region_id integer REFERENCES region, 

floors date,  

budget integer,  

hospital_name text NOT NULL); 

 

CREATE TABLE patients(  

patient_id integer PRIMARY KEY,  

treatment_id integer REFERENCES treatments, 

hospital_id integer PREFERENCES hospitals, 

birth_date date,  

policy integer,  

fio text NOT NULL, 

social_status text); 

 

CREATE TABLE doctors(  

doctor_id integer PRIMARY KEY,  

treatment_id integer REFERENCES treatments, 

hospital_id integer PREFERENCES hospitals, 

specialization text,  

fio text NOT NULL, 

Experience numeric); 