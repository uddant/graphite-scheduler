#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <climits>

using namespace std;

// Class representing a team member
class TeamMember {
private:
    string name;               // Name of the team member
    map<int, int> free_times;  // Map of start:end times when the member is free
    string state;              // Two-letter abbreviation for the team member's US state
    int seniority;             // Seniority level: 1 for junior, 2 for senior

public:
    // Constructor for initializing a team member with name, free times, state, and seniority
    TeamMember(string member_name, map<int, int> member_free_times, string member_state, int member_seniority)
        : name(member_name), free_times(member_free_times), state(member_state), seniority(member_seniority)
    {}

    // Getter functions to access private members
    string getName() const {
        return name;
    }

    map<int, int> getFreeTimes() const {
        return free_times;
    }

    string getState() const {
        return state;
    }

    int getSeniority() const {
        return seniority;
    }

    // Setter functions to modify private members
    void setName(const string& member_name) {
        name = member_name;
    }

    void setFreeTimes(const map<int, int>& member_free_times) {
        free_times = member_free_times;
    }

    void setState(const string& member_state) {
        state = member_state;
    }

    void setSeniority(int member_seniority) {
        seniority = member_seniority;
    }

    // Function to display information about the team member
    void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "State: " << state << endl;
        cout << "Seniority: " << seniority << endl;
        cout << "Free Times: " << endl;
        // Loop through the free times map and print the intervals
        // for (const auto& [s,e] : free_times) {
        //     cout << "  from " << s << " to " << e << " free" << endl;
        // }
    }
};

// Global variables
int N;                            // Number of team members
vector<TeamMember> team_members;   // Vector to hold all team members
set<int> all_start_times;          // Set of all unique start times
bool noOneAvailable;               // Flag to indicate if no one is available for a meeting
vector<int> scoreMatrix;           // Score matrix for evaluating team members based on seniority
int min_juniors;                   // Minimum number of juniors required for the meeting
int min_seniors;                   // Minimum number of seniors required for the meeting
map<string, int> eastern_conversion =
    {
        {"AL", 60}, {"AK", 180}, {"AZ", 180}, {"AR", 120}, {"CA", 180},
        {"CO", 120}, {"CT", 0}, {"DE", 0}, {"FL", 0}, {"GA", 0},
        {"HI", 180}, {"ID", 120}, {"IL", 60}, {"IN", 0}, {"IA", 60},
        {"KS", 60}, {"KY", 0}, {"LA", 60}, {"ME", 0}, {"MD", 0},
        {"MA", 0}, {"MI", 0}, {"MN", 60}, {"MS", 60}, {"MO", 60},
        {"MT", 120}, {"NE", 60}, {"NV", 180}, {"NH", 0}, {"NJ", 0},
        {"NM", 120}, {"NY", 0}, {"NC", 0}, {"ND", 60}, {"OH", 0},
        {"OK", 60}, {"OR", 180}, {"PA", 0}, {"RI", 0}, {"SC", 0},
        {"SD", 60}, {"TN", 60}, {"TX", 60}, {"UT", 120}, {"VT", 0},
        {"VA", 0}, {"WA", 180}, {"WV", 0}, {"WI", 60}, {"WY", 120}
    };

// Function to convert military time in string format to an integer
int milTimeToInt(string time) {
    if (time[0] == '0') {
        time = time.substr(1, 3);  // Remove leading zero if present
    }
    return stoi(time);  // Convert string to integer
}

// Function to add a given number of minutes to a military time value
int addMinutes(int start_time, int add_minutes) {
    int start_minutes = start_time%100;  // Extract the minute part of the time
    int start_hours = start_time/100;    // Extract the hour part of the time
    int add_hours  = add_minutes/60;     // Calculate hours to add
    add_minutes = add_minutes%60;        // Calculate remaining minutes to add

    int final_time = start_hours + add_hours;  // Add hours
    if (start_minutes + add_minutes > 60) {
        final_time++;  // Adjust hour if minutes exceed 60
    }
    return (final_time*100) + (start_minutes+add_minutes)%60;  // Return the final time
}

// Function to input data from stdin format specified in documentation
void inputMembers() {
    string name, state, str_start_time_point, str_end_time_point;
    int n_time_points, start_time_point, end_time_point, seniority;

    // Loop through the number of team members
    for (int i=0; i<N; i++) {
        cin>>name;  // Input team member's name
        cin>>state;  // Input the state abbreviation

        cin>>n_time_points;  // Input the number of free time intervals
        map<int,int> free_times;
        // Input each free time interval
        for (int j=0; j<n_time_points; j++) {
            cin>>str_start_time_point>>str_end_time_point;
            start_time_point = milTimeToInt(str_start_time_point);
            end_time_point = milTimeToInt(str_end_time_point);
            start_time_point = addMinutes(start_time_point, eastern_conversion[state]); // corrects for time zone
            end_time_point = addMinutes(end_time_point, eastern_conversion[state]); // corrects for time zone
            free_times.insert(pair<int,int>(start_time_point,end_time_point));  // Add free time to the map
            all_start_times.insert(start_time_point);  // Add the start time to the set of all start times
        }

        cin>>seniority;  // Input the seniority level
        team_members.push_back(TeamMember(name, free_times, state, seniority));  // Add team member to the vector
    }
}

// Function to check if a team member is available during a given time interval
bool isAvailable(TeamMember member, int start_time, int end_time) {
    auto fts = member.getFreeTimes();  // Get the team member's free times
    map<int, int>::iterator it = fts.upper_bound(start_time);  // Find the first free time that is greater than the start time
    if (it == fts.begin()) {
        return false;  // If first greater time is the very first time => no time exists when free
    }
    it--;   // else go the immediate start time before the first greater one
    if (it->second >= end_time) {
        return true;  // Check if the member is free for the entire interval
    }
    return false;  // Return false if the member is not free
}

// Function to calculate the score of available team members for a given time interval
vector<int> calcScore(int start_time, int end_time) {
    vector<int> score = {0,0,0};   // score[0] is total score, score[1] is junior count, score[2] is senior count

    // Loop through all team members
    for (int i=0; i<N; i++) {
        bool free = isAvailable(team_members[i], start_time, end_time);  // Check if the member is available
        if (free) {
            noOneAvailable = false;  // Set the flag to false since someone is available
            score[0] += scoreMatrix[team_members[i].getSeniority()];  // Add to the total score based on seniority
            score[team_members[i].getSeniority()]++;  // Increment the count of juniors or seniors
        }
    }
    return score;  // Return the score
}

// Function to query the best time slot for a meeting within the given constraints
vector<pair<int, int>> query(int length, int earliest, int latest) {
    if (addMinutes(earliest, length) > latest) {
        return {pair<int,int>(-1,-1)};    // Return error if no valid time slot exists
    }

    noOneAvailable = true;  // Set the flag to true initially

    vector<int> best_times = {-1};  // Initialize best time to -1 (no valid time)
    vector<int> best_score = {INT_MIN,0,0};  // Initialize best score with negative infinity for comparison
    vector<int> current_score;

    // Check the earliest time
    current_score = calcScore(earliest, addMinutes(earliest, length));
    if (current_score[1]>=min_juniors && current_score[2]>=min_seniors) {
        if (current_score[0]>best_score[0]) {
            best_score = current_score;
            best_times.clear();
            best_times.push_back(earliest);
        }
    }

    // Loop through all start times
    set<int>::iterator it = all_start_times.lower_bound(earliest);
    for (; it != all_start_times.end(); ++it) {
        if (addMinutes(*it, length) > latest) {break;}  // Break if the time exceeds the latest allowed time

        current_score = calcScore(*it, addMinutes(*it, length));
        if (current_score[1]>=min_juniors && current_score[2]>=min_seniors) {
            if (current_score[0]>best_score[0]) {
                    best_score = current_score;
                    best_times.clear();
                    best_times.push_back(*it);  // if strictly greater score, reset best list to that time alone
            }
            else if (current_score[0]==best_score[0]) {
                best_times.push_back(*it);
            }
        }
    }

    // generate paired start-end list from starting times list
    vector<pair<int, int>> result;
    for (auto time:best_times) {
        result.push_back(pair<int, int>(time, addMinutes(time, length)));
    }
    return result;  // Return the best time intervals
}

// Main function
int main() {
    cin>>N;  // Input the number of team members

    inputMembers();  // Input the data for all team members

    cout<<"What is the length of the desired meeting (in minutes)? ";
    int length; cin>>length;
    cout<<"\nWhat is the earliest time the meeting can start (in military time, e.g. 0900)";
    string str_earliest; cin>>str_earliest;
    int earliest = milTimeToInt(str_earliest);
    cout<<"\nWhat is the latest time the meeting can end (in military time, e.g. 1700)";
    string str_latest; cin>>str_latest;
    int latest = milTimeToInt(str_latest);
    cout<<"\nWhat is the minimum number of junior attendees required for the meeting?";
    cin>>min_juniors;            // Minimum number of juniors required
    cout<<"\nWhat is the minimum number of senior attendees required for the meeting?";
    cin>>min_seniors;           // Minimum number of seniors required
    scoreMatrix = {0,1,1};    // Score matrix to assign weight to juniors and seniors based on seniority
    cout<<"\nEnter 2 space-separated integers to represent the weightage of a junior and senior attendee respectively to calculate optimal meeting time.";
    cout<<"\nIf you just want to maximize total attendees, enter 1 1.";
    cin>>scoreMatrix[1]>>scoreMatrix[2];

    vector<pair<int,int>> meeting_times = query(length, earliest, latest);  // Query the best meeting time

    // Handle no meeting cases
    if (noOneAvailable) {
        cout<<"There is no one available for a meeting of this length between these times"<<endl;
        return 0;
    }
    if (meeting_times[0].first == -1) {
        cout<<"The length of the meeting is longer than the allowed time span";
        return 0;
    }

    // Print the best meeting time intervals
    cout<<"The following are the optimal meeting times and its attendees:"<<endl;
    for (int i=0; i<meeting_times.size(); i++) {
        cout<<i+1<<": ";
        if (meeting_times[i].first<1000) {cout<<"0";}
        cout<<meeting_times[i].first<<"-";
        if (meeting_times[i].second<1000) {cout<<"0";}
        cout<<meeting_times[i].second<<endl;
        cout<<"Senior Attendees: ";
        for (auto member:team_members) {
            if (member.getSeniority()==2)
                if (isAvailable(member, meeting_times[i].first, meeting_times[i].second))
                    cout<<member.getName()<<" ";
        }
        cout<<endl;
        cout<<"Junior Attendees: ";
        for (auto member:team_members) {
            if (member.getSeniority()==1)
                if (isAvailable(member, meeting_times[i].first, meeting_times[i].second))
                    cout<<member.getName()<<" ";
        }
        cout<<endl;
    }

    return 0;
}