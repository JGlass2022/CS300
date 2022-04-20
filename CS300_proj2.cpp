//
// Jason Glass
// SNHU CS 300, project 2
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;


/// <summary>
/// Course structure
/// </summary>
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
	Course () {
	}
};




/// <summary>
/// Trims the passed string, removing garbage characters and blank spaces at beginning and end
/// </summary>
/// <param name="theString">String to trim</param>
/// <returns>trimmed string</returns>
string trimString(string theString) {

	// Remove anything from start of string that's not a number or letter
	while (!isalnum(theString.at(0))) {
		theString.erase(theString.begin());
	}

	// Remove blank spaces at end of string
	while (isblank(theString.at(theString.size() - 1))) {
		theString.erase(theString.end());
	}

	return theString;
}




/// <summary>
/// Loads the data file and builds the courses vector
/// </summary>
/// <param name="fileName">file to be loaded</param>
/// <returns>vector containing all courses</returns>
vector<Course> loadCourses() {
	// Create a vector that will be loaded with course data and then returned
	vector<Course> courses;

	// Ask the user to enter the file name to load
	cout << "Enter the data file name to load: ";
	string fileName;
	cin >> fileName;

	// Create a file stream and try to open file, check if successful
	ifstream dataFS;
	dataFS.open(fileName);
	if (dataFS.is_open()) {
		cout << "Loading course data..." << endl;

		// Loop until we can't read any more (such as end of file)
		while (dataFS.good()) {
			string curLine;
			getline(dataFS, curLine);

			vector<string> parsedLine;
			size_t pos = 0;

			// Split the line up wherever there's a comma and  put it in parsedLine vector
			while ((pos = curLine.find(',')) != string::npos) {
				parsedLine.push_back(curLine.substr(0, pos));
				curLine.erase(0, pos + 1);
			}
			parsedLine.push_back(curLine.substr(0, curLine.size()));

			// Create a new course and fill it with the data from this data line
			Course newCourse;
			newCourse.courseNumber = trimString(parsedLine.at(0));
			newCourse.courseName = trimString(parsedLine.at(1));

			// See if there are any prerequisites, and if so put them in the prerequisites of course
			vector<string> prerequisites;
			if (parsedLine.size() > 2) {
				for (int i = 2; i < parsedLine.size(); i++) {
					prerequisites.push_back(trimString(parsedLine.at(i)));
				}
			}
			newCourse.prerequisites = prerequisites;

			// Add this course to the course vector
			courses.push_back(newCourse);
		}

		// Close the file stream
		dataFS.close();
		cout << courses.size() << " courses loaded." << endl;
	}
	else {
		cout << "Unable to open the file " << fileName << endl;
	}

	return courses;
}




/// <summary>
/// Partition the vector of coures into two parts, low and high
/// </summary>
/// <param name="courses">Address of the vector<Course> instance to be partitioned</param>
/// <param name="begin">Beginning index to partition</param>
/// <param name="end">Ending index to partition</param>
/// <returns>The new midpoint of the next sorting recursion</returns>
int partition(vector<Course>& courses, int begin, int end) {
	int lowPoint = begin;
	int highPoint = end;

	// pick the middle element as pivot point
	int midPoint = lowPoint + (highPoint - lowPoint) / 2;
	Course pivot = courses.at(midPoint);

	bool done = false;
	while (!done) {
		// keep incrementing low index while bids[low] < bids[pivot], essentially finding
		// a value in the lower half that should be in the upper half
		while (courses.at(lowPoint).courseNumber < pivot.courseNumber) {
			lowPoint++;
		}

		// keep decrementing high index while bids[pivot] < bids[high], finding a
		// value in the upper half that should be in the lower half
		while (courses.at(highPoint).courseNumber > pivot.courseNumber) {
			highPoint--;
		}

		// If zero or one elements remain, then all numbers are partitioned.
		// That is, we didn't find any that needed to be swapped, so we can stop searching
		if (lowPoint >= highPoint) {
			done = true;
		}
		else {
			// We did find two values in the wrong place, so swap numbers[lowPoint]
			// and numbers[highPoint], and continue searching for more items to swap
			iter_swap(courses.begin() + lowPoint, courses.begin() + highPoint);

			lowPoint++;
			highPoint--;
		}
	}

	// Return the last used highPoint, which will serve as the midpoint in the next recursion
	return highPoint;
}




/// <summary>
/// Performs a quick sort based on the course number
/// </summary>
/// <param name="courses">Address of the vector<Course> instance to be partitioned</param>
/// <param name="begin">Start point of where to sort in vector</param>
/// <param name="end">End point of where to sort in vector</param>
void quickSort(vector<Course>& courses, int begin, int end) {

	// Base case: If there are 1 or zero courses to sort, partition is already sorted
	if (begin >= end) {
		return;
	}

	// Get a midpoint from the partition function, which does the actual swapping of values
	int midPoint = partition(courses, begin, end);

	// Recursively quicksort both the upper half of the vector and the lower half
	quickSort(courses, begin, midPoint);
	quickSort(courses, midPoint + 1, end);
}




/// <summary>
/// Prints the information of all courses
/// </summary>
/// <param name="courses">Vector of courses to print</param>
void printAllCourses(vector<Course> courses) {

	// Make sure there are courses 
	if (courses.size() < 1) {
		cout << "No courses available." << endl;
		return;
	}

	// Sort the courses by course number
	quickSort(courses, 0, courses.size() - 1);

	// Loops through the courses, printing the number and name of each
	cout << "Courses:" << endl;
	for (int i = 0; i < courses.size(); i++) {
		Course thisCourse = courses.at(i);
		cout << setw(8) << right << thisCourse.courseNumber << "  " << setw(35) << left << thisCourse.courseName << endl;
	}
}



/// <summary>
/// Checks to see is the two strings match (not case-sensitive)
/// </summary>
/// <param name="string1">first string to compare</param>
/// <param name="string2">second string to compare</param>
/// <returns>Whether the two strings are identical, aside from capitalization</returns>
bool matchString(string string1, string string2) {
	if (string1.size() != string2.size()) {
		return false;
	}

	for (int i = 0; i < string1.size(); i++) {
		if (tolower(string1[i]) != tolower(string2[i])) {
			return false;
		}
	}
	return true;
}



/// <summary>
/// Searches for a course based on a course number
/// </summary>
/// <param name="courses">Vector of courses to search</param>
/// <param name="searchCourse">Course number to search for</param>
/// <returns>Desired course, if it exists. And empty Course object if not</returns>
Course findCourse(vector<Course>& courses, string searchCourse) {

	// Loop through the vector of courses to find one with a matching course number
	for (Course thisCourse : courses) {
		if (matchString (thisCourse.courseNumber, searchCourse)) {
			return thisCourse;
		}
	}

	// No match found, create and return an empty course
	Course emptyCourse;
	return emptyCourse;
}




/// <summary>
/// Asks the user to enter a course number to search for, then prints information about it
/// </summary>
/// <param name="courses">Vector of Course objects to search</param>
void printCourseInformation(vector<Course> &courses) {

	// Make sure there are courses 
	if (courses.size() < 1) {
		cout << "No courses available." << endl;
		return;
	}

	// Allow user to enter a course number to search for
	cout << "What course do you want to know about?" << endl;
	string selectedCourse;
	cin >> selectedCourse;

	// Try to find a course with matching courseNumber
	Course course = findCourse(courses, selectedCourse);
	if (course.courseNumber == "") {
		cout << "Selected course not found." << endl;
		return;
	}

	// Course found, output data about the course
	cout << course.courseNumber << ": " << course.courseName << endl;
	cout << "Prerequisites: ";
	if (course.prerequisites.size() < 1) {
		cout << "none" << endl;
	}
	else {
		// For each prerequisite, try to find that course number and print info about that course
		cout << endl;
		for (int i = 0; i < course.prerequisites.size(); i++) {
			Course prereqCourse = findCourse(courses, course.prerequisites.at(i));
			if (prereqCourse.courseNumber != "") {
				cout << "  " << prereqCourse.courseNumber << ": " << prereqCourse.courseName << endl;
			}
			else
			{
				cout << "Error: Unknown course " << course.prerequisites.at(i) << endl;
			}
		}
	}
}


/// <summary>
/// Entry point for program execution
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[]) {
	vector<Course> courses;

	// Main loop, continues until user quits
	int selectedOption = 0;
	while (selectedOption != 9) {

		// Display the menu
		cout << "Menu:" << endl;
		cout << "1) Load data structure" << endl;
		cout << "2) Print course list" << endl;
		cout << "3) Print course" << endl;
		cout << "9) Exit" << endl;

		// Get the user's selection
		cin >> selectedOption;

		// Prevents infinite loop if user enters a non-numeric character
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// Process the user's selection
		switch (selectedOption) {
		case 1:
			courses = loadCourses();
			break;
		case 2:
			printAllCourses (courses);
			break;
		case 3:
			printCourseInformation(courses);
			break;
		case 9:
			cout << "Have a good day!" << endl;
			break;
		default:
			cout << "Invalid option selected." << endl << endl;
		}
	}
}