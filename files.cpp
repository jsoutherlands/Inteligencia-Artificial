#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "structs.cpp"

using namespace std;

void readExamsFile(string filename, unordered_map<int, Exam>& exams) {
	filename = "Instancias/" + filename + ".exm";
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

void readStudentsFile(string filename, unordered_map<string, Student>& students, unordered_map<int, Exam>& exams) {
	filename = "Instancias/" + filename + ".stu";
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

void writeResFile(string filename, int timeslots){
	filename = "Instancias/" + filename + ".res";
	ofstream resFile(filename);
	if(!resFile.is_open()) {
		cerr << "Error al abrir archivo " << filename << endl;
		return;
	}
	resFile << timeslots << endl;
	resFile.close();
}

void writePenFile(string filename, double penalizacion){
	filename = "Instancias/" + filename + ".pen";
	ofstream resFile(filename);
	if(!resFile.is_open()) {
		cerr << "Error al abrir archivo " << filename << endl;
		return;
	}
	resFile << penalizacion << endl;
	resFile.close();
}

void writeSolFile(string filename, unordered_map<int, int> examSchedule){
	filename = "Instancias/" + filename + ".sol";
	ofstream solFile(filename);
	if(!solFile.is_open()) {
		cerr << "Error al abrir archivo " << filename << endl;
		return;
	}
	for (const auto& pair : examSchedule) {
        solFile << pair.first << " " << pair.second << endl;
    }
	solFile.close();
}
