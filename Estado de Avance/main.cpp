#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "constraints.cpp"

using namespace std;

// Funcion de evaluacion -> penalizacion promedio
double calculaPenalizacion(map<string, Student>& students, const map<int, int>& examTimeslot) {
	double total = 0;
	for (auto& [student_id, student] : students) {
		calculaPenalizacionPorEstudiante(student, examTimeslot);
		total += student.penalty;
	};

	return (total / students.size());
}

tuple<map<int, int>, int, double> initialSolution(map<int, Exam>& exams, map<string, Student>& students) {
	vector<Exam> sortedExams;

	// Ordena examenes de mayor a menor cantidad de estudiantes
	for (const auto& pair : exams) {
		sortedExams.push_back(pair.second);
	}
	sort(sortedExams.begin(), sortedExams.end(), [](const Exam& a, const Exam& b) {
		return a.student_count > b.student_count;
	});

	map<int, int> examTimeslot;
	vector<vector<int>> timeslots;

	for (const Exam& exam: sortedExams) {
		bool assigned = false;
		for (int i = 0; i < timeslots.size(); ++i) {
			if (!hasConflict(exam, timeslots[i], exams)) {
				timeslots[i].push_back(exam.id);
				examTimeslot[exam.id] = i;
				assigned = true;
				break;
			}
		}
		if (!assigned) {
			timeslots.push_back({exam.id});
			examTimeslot[exam.id] = timeslots.size() - 1;
		}
	}
	double promPenalizacion = calculaPenalizacion(students, examTimeslot);
	return {examTimeslot, timeslots.size(), promPenalizacion};
}

int main(int argc, char* argv[]) {
	map <int, Exam> exams;
	map <string, Student> students;


	// Lectura de instancias
	readExamsFile(argv[1], exams);
	readStudentsFile(argv[2], students, exams);

	auto [examSchedule, totalUsedTimeslots, averagePenalty] = initialSolution(exams, students);

    for (const auto& [student_id, student] : students) {
        cout << "Estudiante " << student_id << " -> Penalizacion " << student.penalty << endl;
    }

    for (const auto& pair : examSchedule) {
        cout << "Examen " << pair.first << " -> Timeslot " << pair.second << endl;
    }

    cout << "Timeslots: " << totalUsedTimeslots << endl;
    cout << "Penalizacion promedio: " << averagePenalty << endl;

    // probando cositas
	/*
    for (const auto& pair : examSchedule) {
        cout << "Examen " << pair.first << " -> Timeslot " << pair.second << endl;
    }
	cout << totalUsedTimeslots << endl;*/

	
	/*for (const auto& [key, value] : students) {
		cout << key << " Estudiante " << value.id_student << " " << value.penalty << endl;
		for (int i = 0; i< value.exams.size(); i++) {
			cout << value.exams[i] << endl;
		}
	}*/
	/*
	for (const auto& [key, value] : exams) {
		cout << key << " " << value.id << " " << value.student_count << endl;
		for (int i = 0; i< value.students.size(); i++) {
			cout << value.students[i] << endl;
		}
	}
	*/
}