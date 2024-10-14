#include <string>
#include <vector>
#include <algorithm>

#include "files.cpp"

using namespace std;

// Restriccion: ningún estudiante puede dar dos examenes al mismo tiempo
bool hasConflict(const Exam& exam, const vector<int>& timeslot, const unordered_map<int, Exam>& exams) {
	for (int examId : timeslot) {
		const vector<string>& students_in_slot = exams.at(examId).students;
		for (const string& student : exam.students) {
			if (find(students_in_slot.begin(), students_in_slot.end(), student) != students_in_slot.end()) {
				return true;
			}
		}
	}
	return false;
}

// Penalizacion por estudiante
void calculaPenalizacionPorEstudiante(Student& student, const unordered_map<int, int>& examTimeslot) {
	vector<int> timeslots;
	for (int exam : student.exams) {
		timeslots.push_back(examTimeslot.at(exam));
	}
	sort(timeslots.begin(), timeslots.end());

	int penalty = 0;
	for (size_t i = 0; i < timeslots.size() -1; ++i) {
		int gap = timeslots[i + 1] - timeslots[i];
		if (gap == 1) penalty += 16;
		else if (gap == 2) penalty += 8;
		else if (gap == 3) penalty += 4;
		else if (gap == 4) penalty += 2;
		else if (gap == 5) penalty += 1;
	};

	student.penalty = penalty;
}