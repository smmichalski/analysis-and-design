//Sheryl Michalski
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// ======================= Course Structure =======================

struct Course {
    string number;
    string title;
    vector<string> prerequisites;
};

// ======================= BST Node & Class =======================

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course) {
        if (course.number < node->course.number) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.number << ": " << node->course.title << endl;
            inOrder(node->right);
        }
    }

    Node* searchNode(Node* node, const string& courseNumber) {
        if (node == nullptr) {
            return nullptr;
        }
        if (courseNumber == node->course.number) {
            return node;
        } else if (courseNumber < node->course.number) {
            return searchNode(node->left, courseNumber);
        } else {
            return searchNode(node->right, courseNumber);
        }
    }

    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroy(root);
    }

    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    void PrintInOrder() {
        if (root == nullptr) {
            cout << "No courses loaded." << endl;
            return;
        }
        inOrder(root);
    }

    Course* Search(const string& courseNumber) {
        Node* node = searchNode(root, courseNumber);
        if (node == nullptr) return nullptr;
        return &node->course;
    }

    bool IsEmpty() const {
        return root == nullptr;
    }
};

// ======================= Helper Functions =======================

// Split a line by comma into tokens
vector<string> split(const string& line, char delimiter = ',') {
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, delimiter)) {
        // Trim whitespace
        size_t start = token.find_first_not_of(" \t\r\n");
        size_t end   = token.find_last_not_of(" \t\r\n");
        if (start == string::npos) {
            tokens.push_back("");
        } else {
            tokens.push_back(token.substr(start, end - start + 1));
        }
    }
    return tokens;
}

// Load courses from file into BST
bool loadCourses(const string& filename, BinarySearchTree& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> tokens = split(line, ',');

        if (tokens.size() < 2) {
            cout << "Warning: Invalid line (missing course number or title): " << line << endl;
            continue;
        }

        Course c;
        c.number = tokens[0];
        c.title  = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                c.prerequisites.push_back(tokens[i]);
            }
        }

        bst.Insert(c);
    }

    file.close();
    cout << "Courses loaded successfully from '" << filename << "'." << endl;
    return true;
}

// Print detailed course info: title + prerequisites (numbers and titles)
void printCourseInfo(BinarySearchTree& bst, const string& courseNumber) {
    Course* course = bst.Search(courseNumber);
    if (course == nullptr) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << course->number << ": " << course->title << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites:" << endl;
        for (const string& prereqNum : course->prerequisites) {
            Course* prereqCourse = bst.Search(prereqNum);
            if (prereqCourse != nullptr) {
                cout << "  " << prereqCourse->number << ": " << prereqCourse->title << endl;
            } else {
                cout << "  " << prereqNum << " (course not found in data)" << endl;
            }
        }
    }
}

// ======================= Main Program =======================

int main() {
    BinarySearchTree bst;
    bool dataLoaded = false;

    int choice = 0;
    while (choice != 9) {
        cout << "\n===== ABCU Course Planner =====" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        if (choice == 1) {
            string filename;
            cout << "Enter course data file name: ";
            cin >> filename;
            BinarySearchTree newTree;
            if (loadCourses(filename, newTree)) {
                bst = std::move(newTree); // replace tree with newly loaded one
                dataLoaded = true;
            }
        }
        else if (choice == 2) {
            if (!dataLoaded || bst.IsEmpty()) {
                cout << "No data loaded. Please choose option 1 first." << endl;
            } else {
                cout << "\nCourse List:" << endl;
                bst.PrintInOrder();
            }
        }
        else if (choice == 3) {
            if (!dataLoaded || bst.IsEmpty()) {
                cout << "No data loaded. Please choose option 1 first." << endl;
            } else {
                string courseNumber;
                cout << "Enter course number: ";
                cin >> courseNumber;
                printCourseInfo(bst, courseNumber);
            }
        }
        else if (choice == 9) {
            cout << "Goodbye." << endl;
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}

