//Header Files
#include <iostream>
#include <conio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <TxCoord.h>
#include <stdlib.h>

//Namespace
using namespace std;

//Some More Functions to enhance our Program
#pragma region EnhancementFunctions

//Locate Cursor Position on output screen
void gotoxy(int x, int y)	//cursor position set
{
	COORD coordinates;
	coordinates.X = x;     // defining x-axis 1-80
	coordinates.Y = y;     //defining  y-axis 1-24
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

//to Hold the program until user enters any key
void Hold() {
	system("pause>0");
}

#pragma endregion


//Structures
#pragma region Structures

//Login Detail Structure
struct LoginDetail {
	//2 members
	string LoginEmail;
	string LoginPassword;
};

//Student Detail
struct StudentDetails
{	
	//11 members
	string RollNumber;
	string StdName;
	string FatherName;
	string Dob;
	string Cnic;
	string Nationality;
	string Religion;
	string Gender;
	string Phone;
	string Email;
	string Occupation;
};

//Course Detail
struct Courses
{
	//8 members
	string CourseCode;
	string CourseName;
	string StartDate;
	string EndDate;
	string CourseDuration;
	string InstructorName;
	string MaxEnrollment;
	string CourseFee;
};

//Enroll Course
struct Enroll
{
	//3 members
	string EnrollmentId;
	string RollNumber;
	string CourseCode;
};

#pragma endregion


//Functions Declaration
#pragma region Enrollment

void EnrollCourse();
void AddEnrollment();
int InsertEnrollment(Enroll enroll);
Enroll GetEnrollment(string id);
void ViewEnrollment();
void EditEnrollment();
void UpdateEnrollment(Enroll enroll, string id);
void DeleteEnrollment();
void DeleteEnrollmentDetail(string id);

#pragma endregion

#pragma region Courses

void CoursesDetail();
void AddCourse();
int InsertCourse(Courses course);
void ViewCourse();
Courses GetCourse(string code);
void EditCourse();
void UpdateCourse(Courses course, string code);
void DeleteCourse();
void DeleteCourseDetail(string code);

#pragma endregion

#pragma region StudentDetail

void StudentDetail();
void AddStudent();
int InsertStudent(StudentDetails std);
StudentDetails GetStudent(string RollNumber);
void ViewStudent();
void EditStudent();
void UpdateStudent(StudentDetails std, string RollNumber);
void DeleteStudent();
void DeleteStudentDetail(string RollNumber);

#pragma endregion

#pragma region ManageUser

void ManageUsers();
void AddUser();
int InsertUser(LoginDetail login);
LoginDetail GetUser(string LoginEmail);
void ViewUser();
void ChangePassword();
void UpdateUser(LoginDetail loginUpdated, LoginDetail login);
void DeleteUser();
void DeleteUserDetail(LoginDetail login);

#pragma endregion

#pragma region SignOut

void SignOut();

#pragma endregion

#pragma region MainMenu

void Header();
void MainMenu();

#pragma endregion

#pragma region Login

void Login();
bool ValidateLogin(LoginDetail login);

#pragma endregion


//Main Function
void main()
{
	Login();
	Hold();
}

//Functions Definition - code
#pragma region EnrollmentFunctions

//Enroll Course Menu
void EnrollCourse() {
	char option = 0;
	bool invalidOption;

	do
	{
		system("cls");
		Header();
		gotoxy(52, 6);
		cout << "Enroll Course";
		gotoxy(52, 9);
		cout << "1. Add Enrollment";
		gotoxy(52, 10);
		cout << "2. Edit Enrollment";
		gotoxy(52, 11);
		cout << "3. View Enrollment";
		gotoxy(52, 12);
		cout << "4. Delete Enrollment";
		gotoxy(52, 13);
		cout << "5. Main Menu";

		gotoxy(42, 16);
		cout << "Enter your preferred option(1-5): ";
		cin >> option;
		
		//validate choice
		if (option == '1' || option == '2' || option == '3' || option == '4' || option == '5')
		{
			invalidOption = false;
		}
		else {
			gotoxy(35, 18);
			cout << "Invalid Option selected, Press any key to Try Again.";
			invalidOption = true;
			Hold();
		}
	} while (invalidOption == true);

	//redirect to respective functions
	switch (option)
	{
	case '1':	//Add
		AddEnrollment();
		break;

	case '2':	//Edit
		EditEnrollment();
		break;

	case '3':	//View
		ViewEnrollment();
		break;

	case '4':	//Delete
		DeleteEnrollment();
		break;

	case '5':	//Main Menu
		MainMenu();
		break;
	}
}

//Add Enrollment
void AddEnrollment() {
	//struct obj
	Enroll enroll;

	bool invalid = false;

	cin.ignore();
	do
	{
		system("cls");
		Header();

		gotoxy(48, 5);
		cout << "Add New Enrollment";

		gotoxy(44, 8);
		cout << "Enter Enrollment Id: ";
		getline(cin, enroll.EnrollmentId);

		gotoxy(44, 9);
		cout << "Enter Course Code: ";
		getline(cin, enroll.CourseCode);

		gotoxy(44, 10);
		cout << "Enter Roll Number: ";
		getline(cin, enroll.RollNumber);

		//validate null and insert
		if (enroll.EnrollmentId.length() == 0 || enroll.CourseCode.length() == 0 || enroll.RollNumber.length() == 0) {
			gotoxy(32, 21);
			cout << "Invalid Data, Data cannot be NULL, Press any key to Try Again.";
			invalid = false;
			Hold();
		}
		else
		{
			invalid = true;
			if (InsertEnrollment(enroll) == 0) {
				gotoxy(34, 21);
				cout << "Enrollment Inserted Successfully. Press any key to continue.";
				Hold();
				EnrollCourse();
			}
			else if (InsertEnrollment(enroll) == -1) {
				gotoxy(34, 21);
				cout << "Enrollment already exists. Press any key to continue.";
				Hold();
				EnrollCourse();
			}
			else if (InsertEnrollment(enroll) == 1) {
				gotoxy(34, 21);
				cout << "Course does not exists. Press any key to continue.";
				Hold();
				EnrollCourse();
			}
			else if (InsertEnrollment(enroll) == 2) {
				gotoxy(34, 21);
				cout << "Student does not exists. Press any key to continue.";
				Hold();
				EnrollCourse();
			}
		}

	} while (invalid == false);
}

//Insert Enrollment in file
int InsertEnrollment(Enroll enroll) {
	//-1 = exists, 0 = inserted
	//1 = Course does not exist, 2 = Student does not exist

	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[3];

	//file object
	fstream file;

	StudentDetails std;
	Courses course;

	//Open File in read mode.
	file.open("EnrollCourse.txt", ios::in);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;

		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//check existance to avoid duplication
		if (StrArray[0] == enroll.EnrollmentId)
		{
			return -1;
		}
	}

	//Close Read File;
	file.close();

	//Get Student Detail
	std = GetStudent(enroll.RollNumber);

	//Get Course Detail
	course = GetCourse(enroll.CourseCode);

	//Check if Student Exists
	if (std.RollNumber == "") {
		return 2;
	}

	//Check if Course Exists
	if (course.CourseCode == "") {
		return 1;
	}

	//Append Record
	//Open File in Append Mode
	file.open("EnrollCourse.txt", ios::app);

	//Add(Append) Record
	file << enroll.EnrollmentId << "," << enroll.RollNumber << "," << enroll.CourseCode << endl;

	//file close
	file.close();

	return 0;
}

//Get Enrollment from file
Enroll GetEnrollment(string id) {
	//struct obj
	Enroll enroll;

	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[3];

	//file object
	fstream file;

	//File Open, Mode:Read
	file.open("EnrollCourse.txt", ios::in);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;

		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Match record from file with parameter and assign values from file to structure obj
		if (StrArray[0] == id)
		{
			enroll.EnrollmentId = StrArray[0];
			enroll.RollNumber = StrArray[1];
			enroll.CourseCode = StrArray[2];
		}
	}

	return enroll;
}

//View Enrollment
void ViewEnrollment() {
	//struct obj
	Enroll enroll;
	StudentDetails std;
	Courses course;

	string id;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(47, 5);
		cout << "View Enrollment Detail";

		gotoxy(45, 7);
		cout << "Enter Enrollment Id: ";
		getline(cin, id);

		//get enrollment structure obj
		enroll = GetEnrollment(id);

		//check existance and then display
		if (enroll.EnrollmentId == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Enrollment Id, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;
			
			std = GetStudent(enroll.RollNumber);
			course = GetCourse(enroll.CourseCode);

			gotoxy(48, 9);
			cout << "Enrollment Id: " << enroll.EnrollmentId;

			//Display Course Detail
			gotoxy(18, 11);
			cout << "Course Name: " << course.CourseName;

			gotoxy(18, 12);
			cout << "Start Date: " << course.StartDate;

			gotoxy(18, 13);
			cout << "End Date: " << course.EndDate;

			gotoxy(18, 14);
			cout << "Course Duration (days): " << course.CourseDuration;

			gotoxy(18, 15);
			cout << "Instructor Name: " << course.InstructorName;

			gotoxy(18, 16);
			cout << "Max Enrollment: " << course.MaxEnrollment;

			gotoxy(18, 17);
			cout << "Course Fee: " << course.CourseFee;

			//Display Student Detail
			gotoxy(68, 11);
			cout << "Student Name: " << std.StdName;

			gotoxy(68, 12);
			cout << "Father Name: " << std.FatherName;

			gotoxy(68, 13);
			cout << "Date of Birth: " << std.Dob;

			gotoxy(68, 14);
			cout << "CNIC Number: " << std.Cnic;

			gotoxy(68, 15);
			cout << "Nationality: " << std.Nationality;

			gotoxy(68, 16);
			cout << "Gender: " << std.Gender;

			gotoxy(68, 17);
			cout << "Religion: " << std.Religion;

			gotoxy(68, 18);
			cout << "Phone Number: " << std.Phone;

			gotoxy(68, 19);
			cout << "Email: " << std.Email;

			gotoxy(68, 20);
			cout << "Occupation: " << std.Occupation;

			gotoxy(46, 23);
			cout << "Press any key to continue.";
			Hold();
			EnrollCourse();
		}

	} while (invalid == true);
}

//Edit Enrollment
void EditEnrollment() {
	//struct obj
	Enroll enroll;
	StudentDetails std;
	Courses course;

	string id;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "Edit Enrollment Detail";

		gotoxy(44, 8);
		cout << "Enter Enrollment Id: ";
		getline(cin, id);

		enroll = GetEnrollment(id);

		if (enroll.EnrollmentId == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Enrollment Id, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;

			enroll.EnrollmentId = id;	//Enrollment Id is Primary Key for Enrollment, it cannot be changed

			do
			{
				gotoxy(44, 9);
				cout << "Enter Course Code: ";
				getline(cin, enroll.CourseCode);

				gotoxy(44, 10);
				cout << "Enter Roll Number: ";
				getline(cin, enroll.RollNumber);

				std = GetStudent(enroll.RollNumber);
				course = GetCourse(enroll.CourseCode);

				//Check if Student Exists
				if (std.RollNumber == "") {
					gotoxy(32, 21);
					cout << "Student does not exist, Press any key to Try Again.";
					invalid = true;
					Hold();
				}
				else if (course.CourseCode == "") {	//Check if Course Exists
					gotoxy(32, 21);
					cout << "Course does not exist, Press any key to Try Again.";
					invalid = true;
					Hold();
				}
				else {
					invalid = false;
				}
				

			} while (invalid == true);

			UpdateEnrollment(enroll, id);
			gotoxy(34, 21);
			cout << "Enrollment Updated Successfully. Press any key to continue.";
			Hold();
			EnrollCourse();
		}

	} while (invalid == true);
}

//Update in file
void UpdateEnrollment(Enroll enroll, string id) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[3];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("EnrollCourse.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File - EnrollCourse.txt
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Update record
		if (StrArray[0] == id) {
			temp << enroll.EnrollmentId << "," << enroll.RollNumber << "," << enroll.CourseCode << endl;
		}
		else {
			temp << StrArray[0] << "," << StrArray[1] << "," << StrArray[2] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("EnrollCourse.txt");
	rename("Temp.txt", "EnrollCourse.txt");
}

//Delete Enrollment
void DeleteEnrollment() {
	//struct obj
	Enroll enroll;
	StudentDetails std;
	Courses course;

	string id;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "Delete Enrollment Detail";

		gotoxy(44, 8);
		cout << "Enter Enrollment Id: ";
		getline(cin, id);

		enroll = GetEnrollment(id);

		//check existance and delete
		if (enroll.EnrollmentId == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Enrollment Id, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			DeleteEnrollmentDetail(id);
			gotoxy(34, 21);
			cout << "Enrollment Deleted Successfully. Press any key to continue.";
			Hold();
			EnrollCourse();
		}

	} while (invalid == true);
}

//Delete from file
void DeleteEnrollmentDetail(string id) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[3];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("EnrollCourse.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File - EnrollCourse.txt
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;

		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Delete record - write all record into file except one to be deleted
		if (StrArray[0] != id) {
			temp << StrArray[0] << "," << StrArray[1] << "," << StrArray[2] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("EnrollCourse.txt");
	rename("Temp.txt", "EnrollCourse.txt");
}

#pragma endregion

#pragma region CourseDetailFunctions

//Course Detail Menu
void CoursesDetail() {
	char option = 0;
	bool invalidOption;

	do
	{
		system("cls");

		Header();

		gotoxy(52, 6);
		cout << "Courses Detail";
		gotoxy(52, 9);
		cout << "1. Add Course";
		gotoxy(52, 10);
		cout << "2. Edit Course";
		gotoxy(52, 11);
		cout << "3. View Course";
		gotoxy(52, 12);
		cout << "4. Delete Course";
		gotoxy(52, 13);
		cout << "5. Main Menu";

		gotoxy(42, 16);
		cout << "Enter your preferred option(1-5): ";
		cin >> option;

		//validate user choice
		if (option == '1' || option == '2' || option == '3' || option == '4' || option == '5')
		{
			invalidOption = false;
		}
		else {
			gotoxy(35, 18);
			cout << "Invalid Option selected, Press any key to Try Again.";
			invalidOption = true;
			Hold();
		}
	} while (invalidOption == true);

	//redirect to respective functions
	switch (option)
	{
	case '1':	//Add
		AddCourse();
		break;

	case '2':	//Edit
		EditCourse();
		break;

	case '3':	//View
		ViewCourse();
		break;

	case '4':	//Delete
		DeleteCourse();
		break;

	case '5':	//Main Menu
		MainMenu();
		break;
	}
}

//Add Course
void AddCourse() {
	//struct obj
	Courses course;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(48, 5);
		cout << "Add New Course Detail";

		gotoxy(44, 8);
		cout << "Enter Course Code: ";
		getline(cin, course.CourseCode);

		gotoxy(44, 9);
		cout << "Enter Course Name: ";
		getline(cin, course.CourseName);

		gotoxy(44, 10);
		cout << "Enter Start Date: ";
		getline(cin, course.StartDate);

		gotoxy(44, 11);
		cout << "Enter End Date: ";
		getline(cin, course.EndDate);

		gotoxy(44, 12);
		cout << "Enter Course Duration (days): ";
		getline(cin, course.CourseDuration);

		gotoxy(44, 13);
		cout << "Enter Instructor Name: ";
		getline(cin, course.InstructorName);

		gotoxy(44, 14);
		cout << "Enter Max Enrollment: ";
		getline(cin, course.MaxEnrollment);

		gotoxy(44, 15);
		cout << "Enter Course Fee: ";
		getline(cin, course.CourseFee);
		
		//validate null and insert
		if (course.CourseCode.length() == 0 || course.CourseName.length() == 0 || course.StartDate.length() == 0 || course.EndDate.length() == 0 || course.CourseDuration.length() == 0 || course.InstructorName.length() == 0 || course.MaxEnrollment.length() == 0 || course.CourseFee.length() == 0) {
			gotoxy(32, 21);
			cout << "Invalid Data, Data cannot be NULL, Press any key to Try Again.";
			invalid = false;
			Hold();
		}
		else
		{
			invalid = true;
			if (InsertCourse(course) == 0) {
				gotoxy(34, 21);
				cout << "Course Inserted Successfully. Press any key to continue.";
				Hold();
				CoursesDetail();
			}
			else if (InsertCourse(course) == -1) {
				gotoxy(34, 21);
				cout << "Course already exists. Press any key to continue.";
				Hold();
				CoursesDetail();
			}
		}

	} while (invalid == false);
}

//Insert Course in file
int InsertCourse(Courses course) {
	//-1 = exists, 0=inserted
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[8];

	//file object
	fstream file;


	//Open File in read mode.
	file.open("CourseDetail.txt", ios::in);

	//Read File to check duplicate
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//check if course code already exists to avoid duplication
		if (StrArray[0] == course.CourseCode)
		{
			return -1;
		}
	}

	//Close Read File;
	file.close();

	//Append Record
	//Open File in Append Mode
	file.open("CourseDetail.txt", ios::app);

	//Add(Append) Record
	file << course.CourseCode << "," << course.CourseName << "," << course.StartDate << "," << course.EndDate << "," << course.CourseDuration << "," << course.InstructorName << "," << course.MaxEnrollment << "," << course.CourseFee << endl;

	//close file
	file.close();

	//Redirect to Courses Detail
	return 0;
}

//Get Course by code from file
Courses GetCourse(string code) {
	//struct obj
	Courses course;

	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[8];

	//file object
	fstream file;

	//File Open, Mode:Read
	file.open("CourseDetail.txt", ios::in);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;

		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//check if exists then assign data from file to structure obj
		if (StrArray[0] == code)
		{
			course.CourseCode = StrArray[0];
			course.CourseName = StrArray[1];
			course.StartDate = StrArray[2];
			course.EndDate = StrArray[3];
			course.CourseDuration = StrArray[4];
			course.InstructorName = StrArray[5];
			course.MaxEnrollment = StrArray[6];
			course.CourseFee = StrArray[7];
		}
	}
	return course;
}

//View Course
void ViewCourse() {
	//struct obj
	Courses course;
	string code;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "View Course Detail";

		gotoxy(44, 8);
		cout << "Enter Course Code: ";
		getline(cin, code);

		//get struct obj
		course = GetCourse(code);

		//check existance and display
		if (course.CourseCode == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Course Code, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;
			gotoxy(44, 9);
			cout << "Course Name: " << course.CourseName;

			gotoxy(44, 10);
			cout << "Start Date: " << course.StartDate;

			gotoxy(44, 11);
			cout << "End Date: " << course.EndDate;

			gotoxy(44, 12);
			cout << "Course Duration (days): " << course.CourseDuration;

			gotoxy(44, 13);
			cout << "Instructor Name: " << course.InstructorName;

			gotoxy(44, 14);
			cout << "Max Enrollment: " << course.MaxEnrollment;

			gotoxy(44, 15);
			cout << "Course Fee: " << course.CourseFee;

			gotoxy(46, 20);
			cout << "Press any key to continue.";

			Hold();
			
			CoursesDetail();
		}

	} while (invalid == true);
}

//Edit Course
void EditCourse() {
	//struct obj
	Courses course;

	string code;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "Edit Course Detail";

		gotoxy(44, 8);
		cout << "Enter Course Code: ";
		getline(cin, code);

		//get course struct obj
		course = GetCourse(code);

		//check existance and take updated data from user
		if (course.CourseCode == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Course Code, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;

			course.CourseCode = code;	//Course Code is Primary Key for Courses, it cannot be changed

			do
			{

				gotoxy(44, 9);
				cout << "Enter Course Name: ";
				getline(cin, course.CourseName);

				gotoxy(44, 10);
				cout << "Enter Start Date: ";
				getline(cin, course.StartDate);

				gotoxy(44, 11);
				cout << "Enter End Date: ";
				getline(cin, course.EndDate);

				gotoxy(44, 12);
				cout << "Enter Course Duration (days): ";
				getline(cin, course.CourseDuration);

				gotoxy(44, 13);
				cout << "Enter Instructor Name: ";
				getline(cin, course.InstructorName);

				gotoxy(44, 14);
				cout << "Enter Max Enrollment: ";
				getline(cin, course.MaxEnrollment);

				gotoxy(44, 15);
				cout << "Enter Course Fee: ";
				getline(cin, course.CourseFee);

				//validate null and update course
				if (course.CourseCode.length() == 0 || course.CourseName.length() == 0 || course.StartDate.length() == 0 || course.EndDate.length() == 0 || course.CourseDuration.length() == 0 || course.InstructorName.length() == 0 || course.MaxEnrollment.length() == 0 || course.CourseFee.length() == 0) {
					gotoxy(32, 21);
					cout << "Invalid Data, Data cannot be NULL, Press any key to Try Again.";
					invalid = true;
					Hold();
				}
				else
				{
					invalid = false;
					UpdateCourse(course,code);
					gotoxy(34, 21);
					cout << "Course Updated Successfully. Press any key to continue.";
					Hold();
					CoursesDetail();
				}

			} while (invalid == true);
		}

	} while (invalid == true);
}

//Update Course in file
void UpdateCourse(Courses course, string code) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[8];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("CourseDetail.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Update record
		if (StrArray[0] == code) {
			temp << course.CourseCode << "," << course.CourseName << "," << course.StartDate << "," << course.EndDate << "," << course.CourseDuration << "," << course.InstructorName << "," << course.MaxEnrollment << "," << course.CourseFee << endl;
		}
		else {
			temp << StrArray[0] << "," << StrArray[1] << "," << StrArray[2] << "," << StrArray[3] << "," << StrArray[4] << "," << StrArray[5] << "," << StrArray[6] << "," << StrArray[7] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("CourseDetail.txt");
	rename("Temp.txt", "CourseDetail.txt");
}

//Delete Course
void DeleteCourse() {
	//struct obj
	Courses course;
	string code;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "Delete Course Detail";

		gotoxy(44, 8);
		cout << "Enter Course Code: ";
		getline(cin, code);

		//store course struct obj
		course = GetCourse(code);

		//Check if Record exists or not
		if (course.CourseCode == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Course Code, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;

			DeleteCourseDetail(code);
			gotoxy(34, 21);
			cout << "Course Deleted Successfully. Press any key to continue.";
			Hold();
			CoursesDetail();
		}

	} while (invalid == true);
}

//Delete Course from file
void DeleteCourseDetail(string code) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[8];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("CourseDetail.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Delete Rec - write all records in file except record to be deleted
		if (StrArray[0] != code) {
			temp << StrArray[0] << "," << StrArray[1] << "," << StrArray[2] << "," << StrArray[3] << "," << StrArray[4] << "," << StrArray[5] << "," << StrArray[6] << "," << StrArray[7] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("CourseDetail.txt");
	rename("Temp.txt", "CourseDetail.txt");
}

#pragma endregion

#pragma region StudentDetailFunctions

//Student Detail
void StudentDetail() {

	//to store user choice
	char option = 0;
	
	bool invalidOption;

	//display std detail menu and input user choice
	do
	{
		system("cls");

		Header();

		gotoxy(52, 6);
		cout << "Student Detail";
		gotoxy(52, 9);
		cout << "1. Add Student";
		gotoxy(52, 10);
		cout << "2. Edit Student";
		gotoxy(52, 11);
		cout << "3. View Student";
		gotoxy(52, 12);
		cout << "4. Delete Student";
		gotoxy(52, 13);
		cout << "5. Main Menu";

		//user choice input
		gotoxy(42, 16);
		cout << "Enter your preferred option(1-5): ";
		cin >> option;

		//validate user choice
		if (option == '1' || option == '2' || option == '3' || option == '4' || option == '5')
		{
			invalidOption = false;
		}
		else {
			gotoxy(35, 18);
			cout << "Invalid Option selected, Press any key to Try Again.";
			invalidOption = true;
			Hold();
		}
	} while (invalidOption == true);

	//redirect to respective functions
	switch (option)
	{
	case '1':	//Add
		AddStudent();
		break;

	case '2':	//Edit
		EditStudent();
		break;

	case '3':	//View
		ViewStudent();
		break;

	case '4':	//Delete
		DeleteStudent();
		break;

	case '5':	//Main Menu
		MainMenu();
		break;
	}
}

//Add Student
void AddStudent() {
	//struct obj
	StudentDetails std;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");

		Header();

		gotoxy(48, 5);
		cout << "Add New Student Detail";

		gotoxy(44, 8);
		cout << "Enter Roll Number: ";
		getline(cin, std.RollNumber);

		gotoxy(44, 9);
		cout << "Enter Student Name: ";
		getline(cin, std.StdName);

		gotoxy(44, 10);
		cout << "Enter Father Name: ";
		getline(cin, std.FatherName);

		gotoxy(44, 11);
		cout << "Enter Date of Birth: ";
		getline(cin, std.Dob);

		gotoxy(44, 12);
		cout << "Enter CNIC Number: ";
		getline(cin, std.Cnic);

		gotoxy(44, 13);
		cout << "Enter Nationality: ";
		getline(cin, std.Nationality);

		gotoxy(44, 14);
		cout << "Enter Gender: ";
		getline(cin, std.Gender);

		gotoxy(44, 15);
		cout << "Enter Religion: ";
		getline(cin, std.Religion);

		gotoxy(44, 16);
		cout << "Enter Phone Number: ";
		getline(cin, std.Phone);

		gotoxy(44, 17);
		cout << "Enter Email: ";
		getline(cin, std.Email);

		gotoxy(44, 18);
		cout << "Enter Occupation: ";
		getline(cin, std.Occupation);

		//Validation - if null then display error
		if (std.RollNumber.length() == 0 || std.StdName.length() == 0 || std.FatherName.length() == 0 || std.Dob.length() == 0
			|| std.Cnic.length() == 0 || std.Religion.length() == 0 || std.Gender.length() == 0 || std.Nationality.length() == 0
			|| std.Phone.length() == 0 || std.Email.length() == 0 || std.Occupation.length() == 0) {
			gotoxy(30, 21);
			cout << "Invalid Data Found, Data cannot be NULL, Press any key to Try Again.";
			invalid = false;
			Hold();
		}
		else
		{
			invalid = true;
			
			if (InsertStudent(std)==0) {
				gotoxy(34, 21);
				cout << "Student Inserted Successfully. Press any key to continue.";
				Hold();
				StudentDetail();
			}
			else if(InsertStudent(std)==-1) {
				gotoxy(34, 21);
				cout << "Student already exists. Press any key to continue.";
				Hold();
				StudentDetail();
			}
		}
		
	} while (invalid == false);
}

//Insert Student in file
int InsertStudent(StudentDetails std) {
	//-1 = exists, 0=inserted
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[11];

	//file object
	fstream file;

	//Open File in read mode.
	file.open("StudentDetail.txt", ios::in);

	//Read File to check duplicate
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//check if rollnumber alread exist to avoid duplication
		if (StrArray[0] == std.RollNumber)
		{
			return -1;
		}
	}

	//Close Read File;
	file.close();

	//Append Record
	//Open File in Append Mode
	file.open("StudentDetail.txt", ios::app);

	//Add(Append) Record
	file << std.RollNumber << "," << std.StdName << "," << std.FatherName << "," << std.Dob << "," << std.Cnic << "," << std.Nationality << "," << std.Gender << "," << std.Religion << "," << std.Phone << "," << std.Email << "," << std.Occupation << endl;

	//close file
	file.close();

	//Redirect to Student Detail
	return 0;
}

//Get Student by Roll Number from file
StudentDetails GetStudent(string RollNumber) {
	//struct obj
	StudentDetails std;

	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[11];

	//file object
	fstream file;

	//File Open, Mode:Read
	file.open("StudentDetail.txt", ios::in);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Match roll number from file with roll number passed as parameter
		if (StrArray[0] == RollNumber)
		{
			//assigning values to structure obj
			std.RollNumber = StrArray[0];
			std.StdName = StrArray[1];
			std.FatherName = StrArray[2];
			std.Dob = StrArray[3];
			std.Cnic = StrArray[4];
			std.Nationality = StrArray[5];
			std.Gender = StrArray[6];
			std.Religion = StrArray[7];
			std.Phone = StrArray[8];
			std.Email = StrArray[9];
			std.Occupation = StrArray[10];
		}
	}

	return std;
}

//View Student
void ViewStudent() {

	//struct obj
	StudentDetails std;

	string RollNumber;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "View Student Detail";

		gotoxy(44, 8);
		cout << "Enter Roll Number: ";
		getline(cin, RollNumber);

		//store struct obj
		std = GetStudent(RollNumber);

		//validate existance and display data
		if (std.RollNumber == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Roll Number, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;
			gotoxy(44, 9);
			cout << "Student Name: " << std.StdName;

			gotoxy(44, 10);
			cout << "Father Name: " << std.FatherName;

			gotoxy(44, 11);
			cout << "Date of Birth: " << std.Dob;

			gotoxy(44, 12);
			cout << "CNIC Number: " << std.Cnic;

			gotoxy(44, 13);
			cout << "Nationality: " << std.Nationality;

			gotoxy(44, 14);
			cout << "Gender: " << std.Gender;

			gotoxy(44, 15);
			cout << "Religion: " << std.Religion;

			gotoxy(44, 16);
			cout << "Phone Number: " << std.Phone;

			gotoxy(44, 17);
			cout << "Email: " << std.Email;

			gotoxy(44, 18);
			cout << "Occupation: " << std.Occupation;

			gotoxy(46, 20);
			cout << "Press any key to continue.";
			Hold();
			StudentDetail();
		}

	} while (invalid == true);
}

//Edit Student
void EditStudent() {

	//struct obj
	StudentDetails std;

	string RollNumber;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "Edit Student Detail";

		gotoxy(44, 8);
		cout << "Enter Roll Number: ";
		getline(cin, RollNumber);

		//store struct obj
		std = GetStudent(RollNumber);

		//check existance if exist then update
		if (std.RollNumber == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Roll Number, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;

			std.RollNumber = RollNumber;	//Roll Number is Primary Key for Student, it cannot be changed

			do
			{
				gotoxy(44, 9);
				cout << "Enter Student Name: ";
				getline(cin, std.StdName);

				gotoxy(44, 10);
				cout << "Enter Father Name: ";
				getline(cin, std.FatherName);

				gotoxy(44, 11);
				cout << "Enter Date of Birth: ";
				getline(cin, std.Dob);

				gotoxy(44, 12);
				cout << "Enter CNIC Number: ";
				getline(cin, std.Cnic);

				gotoxy(44, 13);
				cout << "Enter Nationality: ";
				getline(cin, std.Nationality);

				gotoxy(44, 14);
				cout << "Enter Gender: ";
				getline(cin, std.Gender);

				gotoxy(44, 15);
				cout << "Enter Religion: ";
				getline(cin, std.Religion);

				gotoxy(44, 16);
				cout << "Enter Phone Number: ";
				getline(cin, std.Phone);

				gotoxy(44, 17);
				cout << "Enter Email: ";
				getline(cin, std.Email);

				gotoxy(44, 18);
				cout << "Enter Occupation: ";
				getline(cin, std.Occupation);

				//validate null
				if (std.RollNumber.length() == 0 || std.StdName.length() == 0 || std.FatherName.length() == 0 || std.Dob.length() == 0
					|| std.Cnic.length() == 0 || std.Religion.length() == 0 || std.Gender.length() == 0 || std.Nationality.length() == 0
					|| std.Phone.length() == 0 || std.Email.length() == 0 || std.Occupation.length() == 0) {
					gotoxy(30, 21);
					cout << "Data cannot be NULL, Press any key to Try Again.";
					invalid = true;
					Hold();
				}
				else
				{
					invalid = false;
					UpdateStudent(std, RollNumber);
					gotoxy(34, 21);
					cout << "Student Updated Successfully. Press any key to continue.";
					Hold();
					StudentDetail();

				}
			} while (invalid == true);
		}

	} while (invalid == true);
}

//Update Student in file
void UpdateStudent(StudentDetails std, string RollNumber) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[3];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("StudentDetail.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Update record
		if (StrArray[0] == RollNumber) {
			temp << std.RollNumber << "," << std.StdName << "," << std.FatherName << "," << std.Dob << "," << std.Cnic << "," << std.Nationality << "," << std.Gender << "," << std.Religion << "," << std.Phone << "," << std.Email << "," << std.Occupation << endl;
		}
		else {
			temp << StrArray[0] << "," << StrArray[1] << "," << StrArray[2] << "," << StrArray[3] << "," << StrArray[4] << "," << StrArray[5] << "," << StrArray[6] << "," << StrArray[7] << "," << StrArray[8] << "," << StrArray[9] << "," << StrArray[10] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("StudentDetail.txt");
	rename("Temp.txt", "StudentDetail.txt");
}

//Delete Student
void DeleteStudent() {

	//struct obj
	StudentDetails std;
	string RollNumber;

	bool invalid = false;

	cin.ignore();

	do
	{
		system("cls");
		Header();

		gotoxy(49, 5);
		cout << "Delete Student Detail";

		//Get Roll Number to delete
		gotoxy(44, 8);
		cout << "Enter Roll Number: ";
		getline(cin, RollNumber);

		//store struct obj
		std = GetStudent(RollNumber);

		//Check if Record exists if yes then delete
		if (std.RollNumber == "")
		{
			gotoxy(30, 10);
			cout << "Invalid Roll Number, No Record Found, Press any key to Try Again.";
			invalid = true;
			Hold();
		}
		else {
			invalid = false;

			DeleteStudentDetail(RollNumber);
			gotoxy(34, 21);
			cout << "Student Deleted Successfully. Press any key to continue.";
			Hold();
			StudentDetail();
		}

	} while (invalid == true);
}

//Delete Student From File
void DeleteStudentDetail(string RollNumber) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[3];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("StudentDetail.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Delete record - write all records into file except one to be deleted
		if (StrArray[0] != RollNumber) {
			temp << StrArray[0] << "," << StrArray[1] << "," << StrArray[2] << "," << StrArray[3] << "," << StrArray[4] << "," << StrArray[5] << "," << StrArray[6] << "," << StrArray[7] << "," << StrArray[8] << "," << StrArray[9] << "," << StrArray[10] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("StudentDetail.txt");
	rename("Temp.txt", "StudentDetail.txt");
}

#pragma endregion

#pragma region ManageUserFunctions

//Manage Users
void ManageUsers() {

	//char variable to store user choice
	char option = 0;

	//to iterate if user enters invalid option
	bool invalidOption;

	
	cin.ignore();

	//displays Manage Users menu and take user input
	do
	{
		//clear screen
		system("cls");

		
		Header();

		gotoxy(52, 6);
		cout << "Manage Users";

		//prefrences
		gotoxy(52, 9);
		cout << "1. Add User";
		gotoxy(52, 10);
		cout << "2. Change Password";
		gotoxy(52, 11);
		cout << "3. View User";
		gotoxy(52, 12);
		cout << "4. Delete User";
		gotoxy(52, 13);
		cout << "5. Main Menu";

		//input user choice
		gotoxy(42, 16);
		cout << "Enter your preferred option(1-5): ";
		cin >> option;

		//validate user choice
		if (option == '1' || option == '2' || option == '3' || option == '4' || option == '5')
		{
			invalidOption = false;
		}
		else {
			gotoxy(35, 18);
			cout << "Invalid Option selected, Press any key to Try Again.";
			invalidOption = true;
			Hold();
		}
	} while (invalidOption == true);

	//redirect to respective function
	switch (option)
	{
	case '1':	//Add New User
		AddUser();
		break;

	case '2':	//Change Password
		ChangePassword();
		break;

	case '3':	//View User
		ViewUser();
		break;

	case '4':	//Delete User
		DeleteUser();
		break;

	case '5':	//Main Menu
		MainMenu();
		break;
	}
}

//Add New User
void AddUser() {

	//LoginDetail structure object
	LoginDetail login;

	//to store confirm password
	string RePassword;

	bool invalid = false;
	
	cin.ignore();

	//display add user form
	do
	{
		system("cls");

		
		Header();

		gotoxy(52, 6);
		cout << "Add New User";

		//take data input from user
		gotoxy(46, 8);
		cout << "Enter User Email: ";
		getline(cin, login.LoginEmail);

		gotoxy(46, 9);
		cout << "Enter Password: ";
		getline(cin, login.LoginPassword);

		gotoxy(46, 10);
		cout << "Re Enter Password: ";
		getline(cin, RePassword);

		//if password and repassword is equal then insert else display error and take input again
		if (login.LoginPassword == RePassword) {
			if (InsertUser(login) == 1) {	//already exist
				gotoxy(38, 12);
				cout << "User Already exists, Press any key to continue.";
				Hold();
				ManageUsers();
			}
			else if (InsertUser(login) == 0)	//null/empty validation
			{
				gotoxy(38, 12);
				cout << "Login Email or Password cannot be NULL.";
				Hold();
				invalid = true;
			}
			else if (InsertUser(login) == 2) {	//inserted
				gotoxy(38, 12);
				cout << "Record successfully added.";
				Hold();
				ManageUsers();
			}
		}
		else {
			gotoxy(46, 12);
			cout << "Password not Matched";
			gotoxy(46, 13);
			cout << "Press Any key to try again";
			invalid = true;
			Hold();
		}
	} while (invalid == true);
}

//Insert New User in file
int InsertUser(LoginDetail login) {

	//0=data null, 1=exists, 2=inserted

	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[2];

	//file object
	fstream file;


	//Validate NULL
	if (login.LoginEmail.length() == 0 || login.LoginPassword.length() == 0) {
		return 0;
	}

	//Open File in read mode.
	file.open("LoginDetail.txt", ios::in);

	//Read File to check if record already exists
	while (getline(file, record))
	{
		//intially set to zero as index starts from zero
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;

		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}

		//Stores last field of the record
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//check if email already exists to avoid duplication
		if (StrArray[0] == login.LoginEmail)
		{
			return 1;
		}
	}

	//Close Read File;
	file.close();

	//Append Record
	//Open File in Append Mode
	file.open("LoginDetail.txt", ios::app);

	//Add(Append) Record
	file << login.LoginEmail << "," << login.LoginPassword << endl;

	//Redirect to Manage Users
	return 2;
}

//Get User by email from file 
LoginDetail GetUser(string LoginEmail) {
	//structure object
	LoginDetail login;

	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[2];

	//file object
	fstream file;


	//File Open, Mode:Read
	file.open("LoginDetail.txt", ios::in);

	//Read File
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		//Stores last field of the record
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Match email from file with email passed through parameters
		if (StrArray[0] == LoginEmail)
		{
			//assign value to structure object from file
			login.LoginEmail = StrArray[0];
			login.LoginPassword = StrArray[1];
		}
	}

	return login;
}

//View User
void ViewUser() {

	//to store email
	string LoginEmail;

	//structure object
	LoginDetail login;

	cin.ignore();

	//clear screen
	system("cls");

	gotoxy(52, 6);
	cout << "View User";

	//take input email
	gotoxy(42, 8);
	cout << "Search by User Email: ";
	getline(cin, LoginEmail);

	//store structure object 
	login = GetUser(LoginEmail);

	//check if exist if yes then display data else display error msg
	if (login.LoginEmail == "") {
		gotoxy(48, 10);
		cout << "User does not exists";
		gotoxy(45, 11);
		cout << "Press any key to continue";
		Hold();
		ManageUsers();
	}
	else {
		system("cls");
		gotoxy(52, 6);
		cout << "View User";

		gotoxy(45, 8);
		cout << "Login Email: " << login.LoginEmail;
		gotoxy(45, 9);
		cout << "Login Password: " << login.LoginPassword;

		gotoxy(45, 11);
		cout << "Press any key to continue";
		Hold();

		//redirect to Manage Users page
		ManageUsers();
	}
}

//Change Password
void ChangePassword() {
	
	//store user input
	string LoginEmail, LoginPassword, RePassword;
	
	//structure object
	LoginDetail login;
	LoginDetail loginUpdated;
	
	bool invalid = false;
	
	cin.ignore();

	system("cls");

	gotoxy(52, 6);
	cout << "Change Password";

	gotoxy(42, 8);
	cout << "Search by User Email: ";
	getline(cin, LoginEmail);

	//get login detail
	login = GetUser(LoginEmail);

	//check record existance
	if (login.LoginEmail == "") {
		gotoxy(48, 10);
		cout << "User does not exists";
		gotoxy(48, 11);
		cout << "Press any key to continue";
		Hold();
		ManageUsers();
	}
	else {
		do
		{
			system("cls");
			gotoxy(52, 6);
			cout << "Change Password";

			gotoxy(45, 8);
			cout << "Login Email: " << LoginEmail;

			gotoxy(45, 9);
			cout << "Enter Password: ";
			getline(cin, LoginPassword);

			gotoxy(45, 10);
			cout << "Re Enter Password: ";
			getline(cin, RePassword);

			//update record
			if (LoginPassword == RePassword) {
				loginUpdated.LoginEmail = LoginEmail;
				loginUpdated.LoginPassword = LoginPassword;

				UpdateUser(loginUpdated, login);

				gotoxy(45, 12);
				cout << "Password Changed, Press any key to continue.";
				Hold();
				ManageUsers();
			}
			else {
				gotoxy(45, 12);
				cout << "Password not matched, Press any key to try again.";
				invalid = true;
				Hold();
			}
		} while (invalid);
	}
}

//Update User in file
void UpdateUser(LoginDetail loginUpdated, LoginDetail login) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[2];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("LoginDetail.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File - LoginDetail.txt
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Update record
		if (StrArray[0] == login.LoginEmail) {
			temp << loginUpdated.LoginEmail << "," << loginUpdated.LoginPassword << endl;
		}
		else {
			temp << StrArray[0] << "," << StrArray[1] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("LoginDetail.txt");	//delete file
	rename("Temp.txt", "LoginDetail.txt");
}

//Delete User
void DeleteUser()
{
	//store user input data
	string LoginEmail, LoginPassword, RePassword;

	//structure obj
	LoginDetail login;

	bool invalid = false;

	cin.ignore();

	system("cls");

	gotoxy(52, 6);
	cout << "Delete User";

	gotoxy(42, 8);
	cout << "Search by User Email: ";
	getline(cin, LoginEmail);

	//get structure obj
	login = GetUser(LoginEmail);

	//validate existance if exist then delete
	if (login.LoginEmail == "") {
		gotoxy(48, 10);
		cout << "User does not exists";
		gotoxy(45, 11);
		cout << "Press any key to continue";
		Hold();
		ManageUsers();
	}
	else {
		DeleteUserDetail(login);

		gotoxy(42, 12);
		cout << "User Deleted, Press any key to continue.";
		Hold();
		ManageUsers();
	}
}

//Delete User from file
void DeleteUserDetail(LoginDetail login) {
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields - size depends upon number of fields/number of members of respective structure
	string StrArray[2];

	//file object
	fstream file;
	fstream temp;

	//File Open, Mode:Read
	file.open("LoginDetail.txt", ios::in);

	//Temp File Open, Mode:Write
	temp.open("Temp.txt", ios::out);

	//Read File - LoginDetail.txt
	while (getline(file, record))
	{
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;
		//Get Record - one by one
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter and split complete record into fields
			if (ch == ',')
			{
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;
			}
			Index++;
		}
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//Delete record - write all records into temp file except record to be deleted
		if (StrArray[0] != login.LoginEmail) {
			temp << StrArray[0] << "," << StrArray[1] << endl;
		}
	}

	//Close Files
	file.close();
	temp.close();

	remove("LoginDetail.txt");
	rename("Temp.txt", "LoginDetail.txt");
}

#pragma endregion

#pragma region SignOutFunctions

//Sign Out
void SignOut()
{
	//to store user choice of logging in back
	char option;

	cin.ignore();

	system("cls");

	Header();

	gotoxy(38, 8);
	cout << "You have been signed out successfully.";
	
	//input re-login choice from user 
	gotoxy(42, 10);
	cout << "Enter y/Y to Login Again: ";
	cin >> option;

	//if user wants to login again then redirect to login page else exit the program
	if (option == 'y' || option == 'Y') {
		Login();
	}
	else {
		exit(1);
	}
}

#pragma endregion

#pragma region MainMenuFunctions

//Prints App Name
void Header() {

	gotoxy(46, 2);
	cout << "Training Management System";

}

//Main Menu
void MainMenu() {

	//char variable to store user choice
	char option = '0';

	//boolean variable to iterate if user enters invalid choice
	bool invalidOption;

	cin.ignore();

	//display main menu to user and input choice
	do
	{
		//Clear Screen
		system("cls");

		//Display's App Name
		Header();

		//Main Menu
		gotoxy(52, 7);
		cout << "MAIN MENU";

		//prefrences available
		gotoxy(52, 10);
		cout << "1. Student Profile";
		gotoxy(52, 11);
		cout << "2. Courses";
		gotoxy(52, 12);
		cout << "3. Enroll Course";
		gotoxy(52, 13);
		cout << "4. Manage Users";
		gotoxy(52, 14);
		cout << "5. Sign Out";

		//input user choice
		gotoxy(42, 17);
		cout << "Enter your preferred option(1-5): ";
		cin >> option;

		//validate if user enters invalid option
		if (option == '1' || option == '2' || option == '3' || option == '4' || option == '5')
		{
			invalidOption = false;
		}
		else {
			gotoxy(35, 18);
			cout << "Invalid Option selected, Press any key to Try Again.";

			invalidOption = true;

			Hold();
		}

	} while (invalidOption == true);

	//Redirect to respective prefrence functions inputted from user
	switch (option)
	{
	case '1':	//Student Profile
		StudentDetail();
		break;

	case '2':	//Courses
		CoursesDetail();
		break;

	case '3':	//Enroll Course
		EnrollCourse();
		break;

	case '4':	//Manage Users
		ManageUsers();
		break;

	case '5':	//Sign Out
		SignOut();
		break;
	}
}

#pragma endregion

#pragma region LoginFunctions

//Login
void Login()
{
	//Object of LoginDetail structure
	LoginDetail login;

	//boolean variable to iterate if login is failed due to inavalid data entered by user
	//initally set to false
	bool Login = false;	//true or false

	//keyboard buffer clear
	cin.ignore();

	//Login Form
	do
	{
		//Clear Screen
		system("cls");

		//User Input
		gotoxy(48, 8);
		cout << "Login Email: ";
		getline(cin, login.LoginEmail);
		gotoxy(48, 9);
		cout << "Password: ";
		getline(cin, login.LoginPassword);

		//bool a - true or false
		// if(a==true), if(a)
		// if(a==false), !- true=false, false=true, if(!a)
		//Validate Login Detail
		if (!ValidateLogin(login))	//if(validatelogin(login) == false)
		{
			gotoxy(48, 11);
			cout << "Invalid Login Email or Password";
			gotoxy(48, 12);
			cout << "Press any key to try again";

			Login = false;

			Hold();
		}
		else {
			Login = true;

			//Redirect to Main Menu
			MainMenu();
		}

	} while (Login == false);
}

//Validate Login
bool ValidateLogin(LoginDetail login)
{
	//string variable to store single record from file
	string record;

	//Integer variables to help in splitting complete record into fields 
	int StartPos = 0, Index = 0, arrIndex = 0, charcount = 0;

	//String array to store complete record splitted into fields
	string StrArray[3];
	//[0] - email
	//[1] - pass

	//file object
	fstream file;

	//File Open, Mode:Read
	//.open(fileName,mode)	filename = str - mode = in, app(existing file write)
	file.open("LoginDetail.txt", ios::in);

	//Read File
	//getline(fileobj, str)
	while (getline(file, record))
	{
		//intially set to zero as index starts from zero
		charcount = 0;
		StartPos = 0;
		Index = 0;
		arrIndex = 0;

		//Get char - one by one
		//a-z, A-Z, 0-9, sp char
		
		//nabeel@outlook.com,nabeel123 - 27 (19,27
		//nabeel@outlook.com, - 19 -1 18
		//str = nabeel
		//str1 = str.substr(3,3) - eel
		for (char ch : record)
		{
			charcount++;

			//Check for delimeter "," and split complete record into fields
			if (ch == ',')
			{
				//str.substr - split string (startpos, num of char)	char=19 char--
				StrArray[arrIndex] = record.substr(StartPos, charcount - 1);
				arrIndex++;
				StartPos = Index + 1;
				charcount = 0;	//recount char after comma
			}
			Index++;
		}

		//Stores last field of the record
		StrArray[arrIndex] = record.substr(StartPos, record.length());

		//check if login exist in the file and data is accurate according to file
		if (StrArray[0] == login.LoginEmail && StrArray[1] == login.LoginPassword)
		{
			//Login is validated
			return true;
		}
	}

	//Login Validation failed
	return false;
}

#pragma endregion
