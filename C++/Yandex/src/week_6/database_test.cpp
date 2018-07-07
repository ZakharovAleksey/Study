#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

string ParseEvent(istream& in);

int main_test_flow(const string& input, ostream& out){
        Database db;

        istringstream in(input);
        for (string line; getline(in, line); ) {
            istringstream is(line);

            string command;
            is >> command;
            if (command == "Add") {
                const auto date = ParseDate(is);
                const auto event = ParseEvent(is);
                db.Add(date, event);
            } else if (command == "Print") {
                db.Print(out);
            } else if (command == "Del") {
                auto condition = ParseCondition(is);
                auto predicate = [condition](const Date& date, const string& event) {
                    return condition->Evaluate(date, event);
                };
                auto count = db.RemoveIf(predicate);
                out << "Removed " << count << " entries" << endl;
            } else if (command == "Find") {
                auto condition = ParseCondition(is);
                auto predicate = [condition](const Date& date, const string& event) {
                    return condition->Evaluate(date, event);
                };

                const auto entries = db.FindIf(predicate);
                for (const auto& entry : entries) {
                    out << entry << endl;
                }
                out << "Found " << entries.size() << " entries" << endl;
            } else if (command == "Last") {
                try {
                    out << db.Last(ParseDate(is)) << endl;
                } catch (invalid_argument&) {
                    out << "No entries" << endl;
                }
            } else if (command.empty()) {
                continue;
            } else {
                throw logic_error("Unknown command: " + command);
            }
        }
        return 0;
}

void TestDataBaseCondition() {
    {
        //Add
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Print\n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n");
            expected.append( "2017-11-22 event2\n");

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Add(), direct date insertion");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-21 event2\n  \
                                      Print\n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n");
            expected.append( "2017-11-21 event2\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Add(), direct date insertion, add several unique events");
        }
        {
            const string scenario =  "Add 2017-11-21 event0\n  \
                                      Add 2017-11-20 event1\n  \
                                      Add 2017-11-21 event2\n  \
                                      Add 2017-11-22 event3\n  \
                                      Add 2017-11-23 event4\n  \
                                      Add 2017-11-22 event5\n  \
                                      Print\n";
            string expected;
            expected.append( "2017-11-20 event1\n" );
            expected.append( "2017-11-21 event0\n");
            expected.append( "2017-11-21 event2\n");
            expected.append( "2017-11-22 event3\n");
            expected.append( "2017-11-22 event5\n");
            expected.append( "2017-11-23 event4\n");

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Add(), random date insertion, add several unique events");
        }
        {
            const string scenario =  "Add 2017-11-21 event0\n  \
                                      Add 2017-11-20 event1\n  \
                                      Add 2017-11-21 event2\n  \
                                      Add 2017-11-22 event3\n  \
                                      Add 2017-11-23 event4\n  \
                                      Add 2017-11-22 event5\n  \
                                      Add 2017-11-22 event3\n  \
                                      Add 2017-11-22 event5\n  \
                                      Add 2017-11-21 event0\n  \
                                      Add 2017-11-20 event1\n  \
                                      Add 2017-11-21 event2\n  \
                                      Print\n";
            string expected;
            expected.append( "2017-11-20 event1\n" );
            expected.append( "2017-11-21 event0\n");
            expected.append( "2017-11-21 event2\n");
            expected.append( "2017-11-22 event3\n");
            expected.append( "2017-11-22 event5\n");
            expected.append( "2017-11-23 event4\n");

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Add(), random date insertion, add several random events");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Print\n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Add(), random date insertion, add several random events");
        }
//        {
//            const string scenario =  "Add 2017-11-21 event0\n  \
//                                      Add 2017-11-20 event1\n  \
//                                      Add 2017-11-21 event2\n  \
//                                      Add 2017-11-22 \"event3\nevent6\"  \
//                                      Add 2017-11-23 event4\n  \
//                                      Add 2017-11-22 event5\n  \
//                                      Print\n";
//            string expected;
//            expected.append( "2017-11-20 event1\n" );
//            expected.append( "2017-11-21 event0\n");
//            expected.append( "2017-11-21 event2\n");
//            expected.append( "2017-11-22 event3\n");
//            expected.append( "2017-11-22 event6\n");
//            expected.append( "2017-11-22 event5\n");
//            expected.append( "2017-11-23 event4\n");

//            stringstream ss_rec;
//            main_test_flow(scenario, ss_rec);
//            string received = ss_rec.str();

//            AssertEqual(received, expected, "Add(), random date insertion, add several unique events");
//        }
    }
    {
        //Print
        {
            const string scenario =  "Print\n";
            string expected;
            expected.append( "" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Print(), no events");
        }
    }
    {
        //Remove
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date > 2017-11-25\n \
                                      Print\n";
            string expected = "Removed 4 entries\n";
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date  > condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date < 2017-11-25\n \
                                      Print\n";
            string expected = "Removed 6 entries\n";
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date < condition ");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date >= 2017-11-25\n \
                                      Print\n";
            string expected = "Removed 6 entries\n";
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date >= condition ");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date <= 2017-11-25\n \
                                      Print\n";
            string expected = "Removed 8 entries\n";
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date <= condition ");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date == 2017-11-25\n \
                                      Print\n";
            string expected = "Removed 2 entries\n";
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date == condition, from list middle");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date == 2017-11-20\n \
                                      Print\n";
            string expected = "Removed 1 entries\n";
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date == condition, from list beginning");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date == 2017-11-27\n \
                                      Print\n";
            string expected = "Removed 2 entries\n";
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date == condition, from list end");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date != 2017-11-20\n \
                                      Print\n";
            string expected = "Removed 11 entries\n";
            expected.append( "2017-11-20 event0\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date != condition, from list beginning");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date != 2017-11-27\n \
                                      Print\n";
            string expected = "Removed 10 entries\n";
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date != condition, from list end");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del date != 2017-11-25\n \
                                      Print\n";
            string expected = "Removed 10 entries\n";
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), date != condition, from list middle");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del event != \"event0\"\n \
                                      Print\n";
            string expected = "Removed 11 entries\n";
            expected.append( "2017-11-20 event0\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), event != condition, single mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Del event != \"event0\"\n \
                                      Print\n";
            string expected = "Removed 9 entries\n";
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-25 event0\n" );
            expected.append( "2017-11-27 event0\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), event != condition, many times mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Del event == \"event0\"\n \
                                      Print\n";
            string expected = "Removed 1 entries\n";
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), event == condition, single mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Del event == \"event0\"\n \
                                      Print\n";
            string expected = "Removed 3 entries\n";
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), event == condition, many times mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Del\n \
                                      Print\n";
            string expected = "Removed 12 entries\n";

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), no condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Del date > 2017-11-25 AND event == \"event0\"\n \
                                      Print\n";
            string expected = "Removed 1 entries\n";
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event0\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), combined AND condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Del date >= 2017-11-25 OR event == \"event0\"\n \
                                      Print\n";
            string expected = "Removed 7 entries\n";
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), combined OR condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 sport event\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Del date < 2017-11-27 AND (event == \"event0\" OR event == \"sport event\")\n\
                                      Print\n";
            string expected;
            expected.append( "Removed 3 entries\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event0\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), combined with brackets");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 sport event\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Del date > 2017-11-20 AND date <= 2017-11-26 AND event == \"event0\"\n\
                                      Print\n";
            string expected;
            expected.append( "Removed 1 entries\n" );
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 sport event\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event0\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Del(), combined with brackets");
        }
    }
    {
        //Find
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date > 2017-11-25\n \
                                       \n";
            string expected;
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );
            expected.append( "Found 4 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date  > condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date < 2017-11-25\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "Found 6 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date < condition ");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date >= 2017-11-25\n \
                                       \n";
            string expected;
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );
            expected.append( "Found 6 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date  >= condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date <= 2017-11-25\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "Found 8 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date <= condition ");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date == 2017-11-25\n \
                                       \n";
            string expected;
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "Found 2 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date == condition, from list middle");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date == 2017-11-20\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "Found 1 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date == condition, from list beginning");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date == 2017-11-27\n \
                                       \n";
            string expected;
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );
            expected.append( "Found 2 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date == condition, from list end");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date != 2017-11-20\n \
                                       \n";
            string expected;
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );
            expected.append( "Found 11 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date != condition, from list beginning");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date != 2017-11-27\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "Found 10 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date != condition, from list end");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find date != 2017-11-25\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );
            expected.append( "Found 10 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), date != condition, from list middle");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find event != \"event0\"\n \
                                       \n";
            string expected;
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event5\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event11\n" );
            expected.append( "Found 11 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), event != condition, single mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Find event != \"event0\"\n \
                                       \n";
            string expected;
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "Found 9 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), event != condition, many times mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Find event == \"event0\"\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "Found 1 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), event == condition, single mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Find event == \"event0\"\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-25 event0\n" );
            expected.append( "2017-11-27 event0\n" );
            expected.append( "Found 3 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), event == condition, many times mentioned");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Find\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-21 event1\n" );
            expected.append( "2017-11-22 event2\n" );
            expected.append( "2017-11-23 event3\n" );
            expected.append( "2017-11-24 event4\n" );
            expected.append( "2017-11-24 event8\n" );
            expected.append( "2017-11-25 event0\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event0\n" );
            expected.append( "Found 12 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), no condition");
        }
        {
            const string scenario =  "Find\n \
                                       \n";
            string expected;
            expected.append( "Found 0 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), no condition, empty box");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Find date > 2017-11-25 AND event == \"event0\"\n \
                                       \n";
            string expected;
            expected.append( "2017-11-27 event0\n" );
            expected.append( "Found 1 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), combined AND condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Find date >= 2017-11-25 OR event == \"event0\"\n \
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-25 event0\n" );
            expected.append( "2017-11-25 event9\n" );
            expected.append( "2017-11-26 event6\n" );
            expected.append( "2017-11-26 event10\n" );
            expected.append( "2017-11-27 event7\n" );
            expected.append( "2017-11-27 event0\n" );
            expected.append( "Found 7 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), combined OR condition");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 sport event\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Find date < 2017-11-27 AND (event == \"event0\" OR event == \"sport event\")\n\
                                       \n";
            string expected;
            expected.append( "2017-11-20 event0\n" );
            expected.append( "2017-11-23 sport event\n" );
            expected.append( "2017-11-25 event0\n" );
            expected.append( "Found 3 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), combined with brackets");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 sport event\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event0\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event0\n  \
                                      Find date > 2017-11-20 AND date <= 2017-11-26 AND event == \"event0\"\n\
                                       \n";
            string expected;
            expected.append( "2017-11-25 event0\n" );
            expected.append( "Found 1 entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Find(), combined with brackets");
        }
    }
    {
        //Last
        {
            const string scenario =  "Last 2017-11-20\n";
            string expected;
            expected.append( "No entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Last(), empty box");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Last 2017-11-19\n";
            string expected;
            expected.append( "No entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Last(), data before db beginning");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Last 2017-11-20";
            string expected;
            expected.append( "2017-11-20 event0\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Last(), first single db item");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Last 2017-11-25";
            string expected;
            expected.append( "2017-11-25 event9\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Last(), request to the middle");
        }
        {
            const string scenario =  "Add 2017-11-20 event0\n  \
                                      Add 2017-11-21 event1\n  \
                                      Add 2017-11-22 event2\n  \
                                      Add 2017-11-23 event3\n  \
                                      Add 2017-11-24 event4\n  \
                                      Add 2017-11-25 event5\n  \
                                      Add 2017-11-26 event6\n  \
                                      Add 2017-11-27 event7\n  \
                                      Add 2017-11-24 event8\n  \
                                      Add 2017-11-25 event9\n  \
                                      Add 2017-11-26 event10\n  \
                                      Add 2017-11-27 event11\n  \
                                      Last 2017-11-29";
            string expected;
            expected.append( "2017-11-27 event11\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Last(), request to the end");
        }
        {
            const string scenario =  "Last\n";
            string expected;
            expected.append( "No entries\n" );

            stringstream ss_rec;
            main_test_flow(scenario, ss_rec);
            string received = ss_rec.str();

            AssertEqual(received, expected, "Last(), empty date");
        }
//        {
//            const string scenario =  "Add 2017-11-20 event0\n  \
//                    Add 2017-11-21 event1\n  \
//                    Add 2017-11-22 event2\n  \
//                    Add 2017-11-23 event3\n  \
//                    Add 2017-11-24 event4\n  \
//                    Add 2017-11-25 event5\n  \
//                    Add 2017-11-26 event6\n  \
//                    Add 2017-11-27 event7\n  \
//                    Add 2017-11-24 event8\n  \
//                    Add 2017-11-25 event9\n  \
//                    Add 2017-11-26 event10\n  \
//                    Last\n";
//            string expected;
//            expected.append( "No entries\n" );

//            stringstream ss_rec;
//            main_test_flow(scenario, ss_rec);
//            string received = ss_rec.str();

//            AssertEqual(received, expected, "Last(), empty date");
//        }
    }
}

