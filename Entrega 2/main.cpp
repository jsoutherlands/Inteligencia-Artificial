#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

#include "constraints.cpp"

using namespace std;

// Funcion de evaluacion -> penalizacion promedio
double calculaPenalizacion(unordered_map<string, Student>& students, const unordered_map<int, int>& examTimeslot) {
	double total = 0;
	for (auto& [student_id, student] : students) {
		calculaPenalizacionPorEstudiante(student, examTimeslot);
		total += student.penalty;
	};

	return (total / students.size());
}

tuple<unordered_map<int, int>, int, double> initialSolution(unordered_map<int, Exam>& exams, unordered_map<string, Student>& students) {
	vector<Exam> sortedExams;

	// Ordena examenes de mayor a menor cantidad de estudiantes
	for (const auto& pair : exams) {
		sortedExams.push_back(pair.second);
	}
	sort(sortedExams.begin(), sortedExams.end(), [](const Exam& a, const Exam& b) {
		return a.student_count > b.student_count;
	});

	unordered_map<int, int> examTimeslot;
	vector<vector<int>> timeslots;

	// Asigna los timeslots
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

	// Calculo de penalizacion promedio
	double promPenalizacion = calculaPenalizacion(students, examTimeslot);
	return {examTimeslot, timeslots.size(), promPenalizacion};
}

bool trySwap(unordered_map<int, int>& examTimeslot, int exam1, int exam2, const unordered_map<int, Exam>& exams, unordered_map<string, Student>& students) {
	int timeslot1 = examTimeslot[exam1];
	int timeslot2 = examTimeslot[exam2];

	// Verifica no-existencia de conflictos
	if (hasConflict(exams.at(exam1), {exam2}, exams) || hasConflict(exams.at(exam2), {exam1}, exams)) {
		return false;
	} else {
	}

	double currentPenalty = calculaPenalizacion(students, examTimeslot);

	examTimeslot[exam1] = timeslot2;
	examTimeslot[exam2] = timeslot1;

	double newPenalty = calculaPenalizacion(students, examTimeslot);
	
	// Verifica que nueva penalizacion sea válida
	if (newPenalty < currentPenalty) {
		return true;
	} else {
		examTimeslot[exam1] = timeslot1;
		examTimeslot[exam2] = timeslot2;
		return false;
	}
}

pair<double, int> hillClimbing(unordered_map<int, Exam>& exams, unordered_map<string, Student>& students, unordered_map<int, int>& examTimeslot) {
	bool improvement = false;
	int iterations = 0;

	// Ejecuta swap mientras no exista una mejora
	while (!improvement) {
		for (auto it1 = examTimeslot.begin(); it1 != examTimeslot.end(); ++it1) {
			for (auto it2 = examTimeslot.begin(); it2 != examTimeslot.end(); ++it2) {
				iterations++;
				if (trySwap(examTimeslot, it1->first, it2->first, exams, students)) {
					improvement = true;
					break;
				}
			}
			if (improvement) break;
		}
	}
	return {calculaPenalizacion(students, examTimeslot), iterations};
}

int main(int argc, char* argv[]) {
	unordered_map <int, Exam> exams;
	unordered_map <string, Student> students;
	unsigned t0, t1;

	t0 = clock(); // Tiempo inicial

	// Lectura de instancias
	readExamsFile(argv[1], exams);
	readStudentsFile(argv[1], students, exams);

	auto [examSchedule, totalUsedTimeslots, averagePenalty] = initialSolution(exams, students);

	/*
	for (const auto& [student_id, student] : students) {
        cout << "Estudiante " << student_id << " -> Penalizacion " << student.penalty << endl;
    }

    for (const auto& pair : examSchedule) {
        cout << "Examen " << pair.first << " -> Timeslot " << pair.second << endl;
    }*/

    cout << "Penalizacion Promedio (Greedy) : " << averagePenalty << endl;
	
	auto [newPenalty, iterations] = hillClimbing(exams, students, examSchedule);

	t1 = clock(); // Tiempo final

	double time = (double(t1-t0) / CLOCKS_PER_SEC);

    /*for (const auto& [student_id, student] : students) {
        cout << "Estudiante " << student_id << " -> Penalizacion " << student.penalty << endl;
    }

    for (const auto& pair : examSchedule) {
        cout << "Examen " << pair.first << " -> Timeslot " << pair.second << endl;
    }*/

	// Escribe arcvhivos de salida
	writePenFile(argv[1], newPenalty);
	writeResFile(argv[1], totalUsedTimeslots);
	writeSolFile(argv[1], examSchedule);

    cout << "Penalizacion promedio (HCFI)   : " << newPenalty << endl;
	cout << "Iteraciones HCFI : " << iterations << " iteraciones" << endl;

	cout << "\nBloques Horarios     : " << totalUsedTimeslots << " bloques" << endl;
	cout << "Tiempo de ejecucion  : " << time << " [s]" << endl;
	cout << "Porcentaje de mejora : " << -((newPenalty - averagePenalty)/averagePenalty)*100 << " %" <<endl;
}