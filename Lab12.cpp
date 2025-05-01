#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;
using namespace sql::mysql;
using namespace sql;

int main()
{
    const string DB_HOST = "tcp://mysql.eecs.ku.edu:3306";
    const string DB_USER = "348s25_t487c919";
    const string DB_PASS = "Chaz7je3";
    const string DB_NAME = DB_USER;

    MySQL_Driver *driver = nullptr;
    Connection *con = nullptr;
    Statement *stmt = nullptr;
    ResultSet *res = nullptr;

    try
    {
        // get driver instance
        driver = mysql::get_mysql_driver_instance();

        // connect
        cout << "Connecting to database at " << DB_HOST << "..." << endl;
        con = driver->connect(DB_HOST, DB_USER, DB_PASS);
        cout << "Connection successful" << endl;

        // select schema
        con->setSchema(DB_NAME);
        cout << "Database schema '" << DB_NAME << "' selected" << endl;

        // execute queries
        cout << "\n--- Starting query execution ---" << endl;

        cout << "Query 1: Retrieve Students with a GPA higher than 3.7, displaying their name, city, and GPA" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "Select StdFirstName, StdLastName, StdCity, StdGPA "
                         "FROM Student "
                         "WHERE StdGPA > 3.7 "
                         "ORDER BY StdLastName, StdFirstName";
            res = stmt->executeQuery(sql);
            cout << "   Results:" << endl;
            bool foundStudents = false;
            while (res->next())
            {
                foundStudents = true;
                string firstName = res->getString("StdFirstName");
                string lastName = res->getString("StdLastName");
                string city = res->getString("StdCity");
                double gpa = res->getDouble("StdGPA");
                cout << "    Name: " << firstName << " " << lastName
                     << ", City: " << city;
                cout << fixed << setprecision(2);
                cout << ", GPA: " << gpa << endl;
                cout.unsetf(ios_base::floatfield);
                cout << setprecision(6);
            }
            if (!foundStudents)
            {
                cout << "    No students found with GPA > 3.7" << endl;
            }

            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 1: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 2: Display Student First Name, Last Name and GPA of students in descending order of GPA" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "Select StdFirstName, StdLastName, StdGPA "
                         "FROM Student "
                         "ORDER BY StdGPA DESC";
            res = stmt->executeQuery(sql);
            cout << "  Results:" << endl;
            bool foundStudents = false;
            while (res->next())
            {
                foundStudents = true;
                string firstName = res->getString("StdFirstName");
                string lastName = res->getString("StdLastName");
                double gpa = res->getDouble("StdGPA");
                cout << "    Name: " << firstName << " " << lastName;
                cout << fixed << setprecision(2);
                cout << ", GPA: " << gpa << endl;
                cout.unsetf(ios_base::floatfield);
                cout << setprecision(6);
            }
            if (!foundStudents)
            {
                cout << "    No students found in the table." << endl;
            }
            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 2: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 3: Retrieve all the information about senior-level IS courses" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "SELECT * "
                         "FROM Course "
                         "WHERE CourseNo LIKE 'IS4%' "
                         "ORDER BY CourseNo";
            res = stmt->executeQuery(sql);
            cout << "  Results:" << endl;
            bool foundCourses = false;
            while (res->next())
            {
                foundCourses = true;
                string courseNo = res->getString("CourseNo");
                string description = res->getString("CrsDesc");
                cout << "    CourseNo: " << courseNo
                     << ", Description: " << description << endl;
            }
            if (!foundCourses)
            {
                cout << "    No senior-level (IS-4XX) IS courses found." << endl;
            }
            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 3: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 4: Summer offering without instructor" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "SELECT OfferNo, CourseNo "
                         "FROM Offering "
                         "WHERE OffTerm = 'SUMMER' AND FacNo IS NULL "
                         "ORDER BY OfferNo";
            res = stmt->executeQuery(sql);
            cout << "  Results:" << endl;
            bool foundOfferings = false;
            while (res->next())
            {
                foundOfferings = true;
                string offerNo = res->getString("OfferNo");
                string courseNo = res->getString("CourseNo");
                cout << "    Offering #: " << offerNo
                     << ", Course #: " << courseNo << endl;
            }
            if (!foundOfferings)
            {
                cout << "    No summer offerings found without an assigned instructor." << endl;
            }
            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 4: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 5: Spring Offerings by assistant faculty" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "SELECT o.OfferNo, o.CourseNo, f.FacFirstName, f.FacLastName "
                         "FROM Offering o "
                         "JOIN Faculty f ON o.FacNo = f.FacNo "
                         "WHERE o.OffTerm = 'SPRING' AND f.FacRank = 'ASST' "
                         "ORDER BY o.OfferNo";
            res = stmt->executeQuery(sql);
            cout << "  Results:" << endl;
            bool foundOfferings = false;
            while (res->next())
            {
                foundOfferings = true;
                string offerNo = res->getString("OfferNo");
                string courseNo = res->getString("CourseNo");
                string facFirstName = res->getString("FacFirstName");
                string facLastName = res->getString("FacLastName");

                cout << "    OfferNo: " << offerNo
                     << ", CourseNo: " << courseNo
                     << ", Instructor: " << facFirstName << " " << facLastName << endl;
            }
            if (!foundOfferings)
            {
                cout << "    No spring offerings taught by assistant faculty found." << endl;
            }
            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 5: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 6: Display Distinct Faculty number in Offering Table" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "SELECT DISTINCT FacNo "
                         "FROM Offering "
                         "ORDER by FacNo";
            res = stmt->executeQuery(sql);
            cout << "  Results (Distinct Faculty Numbers):" << endl;
            bool foundFaculty = false;
            while (res->next())
            {
                foundFaculty = true;
                string facNo;
                if (res->isNull("FacNo"))
                {
                    facNo = "<None/NULL>";
                }
                else
                {
                    facNo = res->getString("FacNo");
                }
                cout << "    Faculty #: " << facNo << endl;
            }
            if (!foundFaculty)
            {
                cout << "    No distinct faculty numbers found in the Offering table." << endl;
            }
            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 6: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 7: Students in Bothell OR Faculty in Bellevue" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "SELECT StdFirstName AS FirstName "
                         "FROM Student "
                         "WHERE StdCity = 'Bothell' "
                         "UNION "
                         "SELECT FacFirstName AS FirstName "
                         "FROM Faculty "
                         "WHERE FacCity = 'Bellevue' "
                         "ORDER BY FirstName";
            res = stmt->executeQuery(sql);
            cout << "  Results (First Names):" << endl;
            bool foundNames = false;
            while (res->next())
            {
                foundNames = true;
                string firstName = res->getString("FirstName");
                cout << "    Name: " << firstName << endl;
            }
            if (!foundNames)
            {
                cout << "    No students found in Bothell OR faculty found in Bellevue." << endl;
            }
            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 7: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 8: Inserting John Doe" << endl;
        try
        {
            stmt = con->createStatement();
            string sql_insert = "INSERT INTO Student "
                                "(StdNo, StdFirstName, StdLastName, StdCity, StdState, StdZip, StdMajor, StdClass, StdGPA) "
                                "VALUES "
                                "('999-99-9999', 'John', 'Doe', 'Lawrence', 'KS', '66045', 'FIN', 'FR', 4.0)";
            cout << "  Executing Insert: " << sql_insert << endl;
            int rowsAffected = stmt->executeUpdate(sql_insert);
            if (rowsAffected > 0)
            {
                cout << "  Successfully inserted John Doe. Rows affected: " << rowsAffected << endl;
                delete stmt;
                stmt = nullptr;
                cout << "  Verifying insertion..." << endl;
                stmt = con->createStatement();
                string sql_select = "SELECT * FROM Student ORDER BY StdLastName, StdFirstName";
                res = stmt->executeQuery(sql_select);
                cout << "  Result:" << endl;
                while (res->next())
                {
                    cout << "    StdNo: " << res->getString("StdNo") << endl;
                    cout << "      Name: " << res->getString("StdFirstName") << " " << res->getString("StdLastName") << endl;
                    cout << "      City: " << res->getString("StdCity") << endl;
                    cout << "      State: " << res->getString("StdState") << endl;
                    cout << "      Zip: " << res->getString("StdZip") << endl;
                    cout << "      Major: " << res->getString("StdMajor") << endl;
                    cout << "      Class: " << res->getString("StdClass") << endl;
                    cout << fixed << setprecision(2);
                    cout << "      GPA: " << res->getDouble("StdGPA") << endl;
                    cout.unsetf(ios_base::floatfield);
                    cout << setprecision(6);
                }
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 8: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 9: Update Homer Wells Major" << endl;
        try
        {
            stmt = con->createStatement();
            string newMajor = "ACCT";
            string firstName = "Homer";
            string lastName = "Wells";
            string sql_update = "UPDATE Student "
                                "SET StdMajor = '" +
                                newMajor + "' "
                                           "WHERE StdFirstName = '" +
                                firstName + "' AND StdLastName = '" + lastName + "'";
            cout << "  Executing Update: " << sql_update << endl;
            int rowsAffected = stmt->executeUpdate(sql_update);
            if (rowsAffected > 0)
            {
                cout << "  Successfully updated Homer Wells' major. Rows affected: " << rowsAffected << endl;
                delete stmt;
                stmt = nullptr;
                stmt = con->createStatement();
                string sql_select = "SELECT * FROM Student ORDER BY StdLastName, StdFirstName";
                res = stmt->executeQuery(sql_select);
                while (res->next())
                {
                    cout << "    StdNo: " << res->getString("StdNo") << endl;
                    cout << "      Name: " << res->getString("StdFirstName") << " " << res->getString("StdLastName") << endl;
                    cout << "      City: " << res->getString("StdCity") << endl;
                    cout << "      State: " << res->getString("StdState") << endl;
                    cout << "      Zip: " << res->getString("StdZip") << endl;
                    cout << "      Major: " << res->getString("StdMajor") << endl;
                    cout << "      Class: " << res->getString("StdClass") << endl;
                    cout << fixed << setprecision(2);
                    cout << "      GPA: " << res->getDouble("StdGPA") << endl;
                    cout.unsetf(ios_base::floatfield);
                    cout << setprecision(6);
                }
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 9: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }

        cout << "Query 10: Students who are also faculty" << endl;
        try
        {
            stmt = con->createStatement();
            string sql = "SELECT s.* "
                         "FROM Student s "
                         "INNER JOIN Faculty f ON s.StdNo = f.FacNo "
                         "ORDER BY s.StdLastName, s.StdFirstName";
            res = stmt->executeQuery(sql);
            cout << "  Results (Student Information for those also in Faculty):" << endl;
            bool foundMatches = false;
            while (res->next())
            {
                foundMatches = true;
                cout << "    StdNo: " << res->getString("StdNo") << endl;
                cout << "      Name: " << res->getString("StdFirstName") << " " << res->getString("StdLastName") << endl;
                cout << "      City: " << res->getString("StdCity") << endl;
                cout << "      State: " << res->getString("StdState") << endl;
                cout << "      Zip: " << res->getString("StdZip") << endl;
                cout << "      Major: " << res->getString("StdMajor") << endl;
                cout << "      Class: " << res->getString("StdClass") << endl;
                cout << fixed << setprecision(2);
                cout << "      GPA: " << res->getDouble("StdGPA") << endl;
                cout.unsetf(ios_base::floatfield);
                cout << setprecision(6);
            }
            if (!foundMatches)
            {
                cout << "    No individuals found who are listed in both the Student and Faculty tables." << endl;
            }
            delete res;
            res = nullptr;
            delete stmt;
            stmt = nullptr;
        }
        catch (SQLException &e)
        {
            cerr << "   Error in Query 10: " << e.what() << endl;
            if (res)
            {
                delete res;
                res = nullptr;
            }
            if (stmt)
            {
                delete stmt;
                stmt = nullptr;
            }
        }
    }
    catch (SQLException &e)
    {
        cerr << e.what() << endl;
        if (res)
        {
            delete res;
        }
        if (stmt)
        {
            delete stmt;
        }
        if (con)
        {
            delete con;
        }
        return 1;
    }
}
