#include <vector>
#include <map>
#include <string>

using namespace std;

struct Exam {
	int id;
	int student_count;
	vector<string> students;
};

struct Student {
	string id_student;
	vector<int> exams;
	int penalty;
};
