#include<bits/stdc++.h>
using namespace std;
#define V 1000

int graph[V][V];
int rGraph[V][V];
int parent[V];
bool visited[V];

bool bfs(int s, int t) {
    memset(visited, 0, sizeof(visited));
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v = 0; v < V; v++) {
            if (visited[v] == false && rGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return (visited[t] == true);
}

void dfs(int s) {
    visited[s] = true;
    for (int i = 0; i < V; i++)
        if (rGraph[s][i] && !visited[i])
            dfs(i);
}

int maxFlow(int s, int t) {
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];
    int max_flow = 0;
    while (bfs(s, t)) {
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}

void addProfessor(int &nTeachers, int workloadCapacity, vector<int> subjects) {
    // Increment the number of teachers by 1 and update the file
    nTeachers++;
    ofstream file1("Number_of_Professors.txt");
    if (file1.is_open()) {
        file1 << nTeachers;
        file1.close();
    }

    // Update the file containing information about teachers
    ofstream file4("Teacher_Id_Workload_Capacity_and_the_course_they_teach.txt", ios::app);
    if (file4.is_open()) {
        file4 << endl << nTeachers << " " << workloadCapacity;
        for (int subjectId : subjects) {
            file4 << " " << subjectId;
        }
        file4.close();
    }
}


int main() {
    int nTeachers, nSubjects;
    ifstream file1("Number_of_Professors.txt");
    if (file1.is_open()) {
        file1 >> nTeachers;
        file1.close();
    }
    ifstream file2("Number_of_Courses.txt");
    if (file2.is_open()) {
        file2 >> nSubjects;
        file2.close();
    }

    int s = 0, t = nTeachers + nSubjects + 1;

    for (int i = 1; i <= nTeachers; i++) {
        int workloadCapacity;
        ifstream file4("Teacher_Id_Workload_Capacity_and_the_course_they_teach.txt");
        if (file4.is_open()) {
            string line;
            while (getline(file4, line)) {
                stringstream ss(line);
                int teacherId;
                ss >> teacherId;
                if (teacherId == i) {
                    ss >> workloadCapacity;
                    graph[s][i] = workloadCapacity;
                    break;
                }
            }
            file4.close();
        }
    }

    for (int i = 1; i <= nSubjects; i++) {
        int subjectHours;
        ifstream file3("Course_Names_with_number_of_hours_for_each_course.txt");
        if (file3.is_open()) {
            string line;
            int subjectId = 0;
            while (getline(file3, line)) {
                subjectId++;
                if (subjectId == i) {
                    stringstream ss(line);
                    string subjectName;
                    ss >> subjectName >> subjectHours;
                    graph[nTeachers + i][t] = subjectHours;
                    break;
                }
            }
            file3.close();
        }
    }

    for (int i = 1; i <= nTeachers; i++) {
        ifstream file4("Teacher_Id_Workload_Capacity_and_the_course_they_teach.txt");
        if (file4.is_open()) {
            string line;
            while (getline(file4, line)) {
                stringstream ss(line);
                int teacherId, workloadCapacity, subjectId;
                ss >> teacherId >> workloadCapacity;
                if (teacherId == i) {
                    while (ss >> subjectId) {
                        graph[i][nTeachers + subjectId] = INT_MAX;
                    }
                    break;
                }
            }
            file4.close();
        }
    }

    int max_flow = maxFlow(s, t);
    cout << "Maximum flow: " << max_flow << endl;
    cout << "Teacher assignments:" << endl;
    for (int i = 1; i <= nTeachers; i++) {
        for (int j = 1; j <= nSubjects; j++) {
            if (graph[i][nTeachers + j] - rGraph[i][nTeachers + j] > 0) {
                cout << "Teacher " << i << " assigned to subject " << j << endl;
            }
        }
    }

    char choice; // Declare a variable to store the user's choice
    cout << "Do you want to add new professors? (y/n): "; // Prompt the user to enter their choice
    cin >> choice; // Read the user's choice from standard input

    if (choice == 'y' || choice == 'Y') { // If the user wants to add new professors
        int n; // Declare a variable to store the number of professors to add
        cout << "Enter the number of professors to add: "; // Prompt the user to enter the number of professors to add
        cin >> n; // Read the number of professors from standard input

        for (int i = 0; i < n; i++) { // For each professor to add
            int workloadCapacity; // Declare a variable to store the workload capacity of the professor
            cout << "Enter the workload capacity of professor " << i + 1 << ": "; // Prompt the user to enter the workload capacity of the professor
            cin >> workloadCapacity; // Read the workload capacity from standard input

            int m; // Declare a variable to store the number of subjects that the professor can teach
            cout << "Enter the number of subjects that professor " << i + 1 << " can teach: "; // Prompt the user to enter the number of subjects that the professor can teach
            cin >> m; // Read the number of subjects from standard input

            vector<int> subjects(m); // Declare a vector to store the subjects that the professor can teach
            cout << "Enter the subjects that professor " << i + 1 << " can teach: "; // Prompt the user to enter the subjects that the professor can teach
            for (int j = 0; j < m; j++) { // For each subject that the professor can teach
                cin >> subjects[j]; // Read a subject from standard input and store it in subjects
            }

            addProfessor(nTeachers, workloadCapacity, subjects); // Call addProfessor function to add a new professor with given details
        }

        // ...
        // Read data from files again and reconstruct graph
        // ...

        // Find updated maximum flow and teacher assignments
        max_flow = maxFlow(s, t);
        cout << "Maximum flow: " << max_flow << endl;
        cout << "Teacher assignments:" << endl;
        cout << "Teacher assignments:" << endl;
        for (int i = 1; i <= nTeachers; i++) {
            for (int j = 1; j <= nSubjects; j++) {
                if (graph[i][nTeachers + j] - rGraph[i][nTeachers + j] > 0) {
                    cout << "Teacher " << i << " assigned to subject " << j << endl;
                }
            }
        }
    }
    return 0;
}
