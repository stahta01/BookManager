#pragma once
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <string>
#include <ctime>
#include <optional>
#include <functional>

namespace BookManager
{
    namespace Entity
    {
        class BookSeries;
        class Person;
        class Publisher;
    }
    namespace Category
    {
        class Category;
    }
    namespace Book
	{
        namespace Abstraction
        {
            class Book;
        }
    }

    namespace Manager
    {
        class TableDelete
        {
            public:
                TableDelete(std::shared_ptr<SQLite::Database> database) : database(database)
                {}

                bool checkAndDeleteInPersonTable(int, bool);
                bool checkAndDeleteInPublisherTable(int, bool);
                bool checkAndDeleteInCategoryTable(int, bool);
                bool checkAndDeleteInBookSeriesTable(int, bool);
                bool deleteInBooksTable(int);

            private:
                bool deleteInPersonTable(int);
                bool deleteInPublisherTable(int);
                bool deleteInCategoryTable(int);
                bool deleteInBookSeriesTable(int);
                std::string getName(SQLite::Statement&, int);
                std::string getPersonName(int);
                bool deleteInTable(SQLite::Statement&, int);

                std::shared_ptr<SQLite::Database> database;
        };

    } // namespace Manager
} // namespace BookManager