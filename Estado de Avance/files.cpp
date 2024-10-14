#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "structs.cpp"

using namespace std;

void readExamsFile(string filename, map<int, Exam>& exams) {
	ifstream examsFile(filename, ios::in);

	if (!examsFile.is_open()) {
		cerr << "Error al abrir archivo " << filename << endl;
		return;
	}

	int id_exam, students_count;
	while (examsFile >> id_exam >> students_count) {
		exams[id_exam] = {id_exam, students_count};
	}
	
	examsFile.close();
}

void readStudentsFile(string filename, map<string, Student>& students, map<int, Exam>& exams) {
	ifstream studentsFile(filename, ios::in);

	if(!studentsFile.is_open()) {
		cerr << "Error al abrir archivo " << filename << endl;
		return;
	}

	string id_stu;
	int id_exam;

	while (studentsFile >> id_stu >> id_exam) {
		students[id_stu].id_student = id_stu;
		students[id_stu].exams.push_back(id_exam);
		exams[id_exam].students.push_back(id_stu);
	}

	studentsFile.close();
}