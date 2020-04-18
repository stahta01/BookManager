#include "Managers/Utils/TableModifier.hpp"
#include "BookAbstract/Book.hpp"
#include "BookEnum/BookType.hpp"
#include "BookFactory/BookFactory.hpp"
#include "EntityTypes/BookSerie.hpp"
#include "EntityTypes/Person.hpp"
#include "EntityTypes/Publisher.hpp"
#include "Utils/EnumUtils.hpp"
#include "Utils/Log.hpp"
#include "Utils/Exceptions.hpp"
#include "../../../Category.hpp"
#include <iostream>
#include <sqlite3.h>

namespace BookManager
{
    namespace Manager
    {
        bool TableModifier::modifyPersonTable(BookManager::Entity::Person personToAdd, SQLite::Statement& query)
        {
            query.bind(":first_name", personToAdd.getFirstName());
            query.bind(":last_name", personToAdd.getLastName());
            query.bind(":role", static_cast<int>(personToAdd.getRole()));

            return query.exec() > 0;
        }

        bool TableModifier::modifyPublisherTable(BookManager::Entity::Publisher publisherToAdd, SQLite::Statement& query)
        {
            query.bind(":name", publisherToAdd.getName());

            return query.exec() > 0;
        }

        bool TableModifier::modifyCategoryTable(BookManager::Category::Category categoryToAdd, SQLite::Statement& query)
        {
            query.bind(":name", categoryToAdd.getName());

            return query.exec() > 0;
        }

        bool TableModifier::modifyBookSerieTable(BookManager::Entity::BookSerie bookSeriesToAdd, SQLite::Statement& query)
        {
            query.bind(":name", bookSeriesToAdd.getName());

            return query.exec() > 0;
        }

        void TableModifier::bindOptionalDate(SQLite::Statement& query, std::string bindName, std::optional<time_t> date)
        {
            if(date)
                query.bind(bindName, convertDateToString(date.value())); // YYY-MM-DD
            else
                query.bind(bindName); // bind to null
        }

        std::string TableModifier::convertDateToString(std::time_t date)
        {
            std::tm* dateTm{localtime(&date)};
            char buffer [80];
            strftime (buffer,80,"%Y-%m-%d",dateTm);
            return std::string(buffer);
        }

        void TableModifier::modifyBooksPersonsTable(int bookId, std::vector<std::shared_ptr<Entity::Person>> persons, SQLite::Statement& query)
        {
            for(auto person : persons)
            {
                try
                {
                    query.bind(":book_id", bookId);
                    query.bind(":person_id", person->getId());
                    query.exec();
                    query.reset();
                }
                catch (const SQLite::Exception &e) {
                    if (e.getExtendedErrorCode() == SQLITE_CONSTRAINT_FOREIGNKEY)
                    {
                        LOG_WINDOW("Error occured with author : The author \"{}, {}\" doesn't exist.",
                            person->getLastName(), person->getFirstName())
                        throw;
                    }
                    LOG_WINDOW("Error occured with author : {}", e.what())
                    throw;
                }
                catch(const std::exception& e)
                {
                    LOG_WINDOW("Error occured with author : {}", e.what())
                    throw;
                }
            }
        }

        void TableModifier::modifyBooksSubCategoriesTable(int bookId, std::vector<std::shared_ptr<Category::Category>> subCategories, SQLite::Statement& query)
        {
            for(auto subCategorie : subCategories)
            {
                try
                {
                    query.bind(":book_id", bookId);
                    query.bind(":subCategory_id", subCategorie->getId());
                    query.exec();
                    query.reset();
                }
                catch (const SQLite::Exception &e) {
                    if (e.getExtendedErrorCode() == SQLITE_CONSTRAINT_FOREIGNKEY)
                    {
                        LOG_WINDOW("Error occured with Subcategories : The Subcategory \"{}\" doesn't exist.",
                            subCategorie->getName())
                        throw;
                    }
                    LOG_WINDOW("Error occured with Subcategories : {}", e.what());
                    throw;
                }
                catch(const std::exception& e)
                {
                    LOG_WINDOW("Error occured with Subcategories : {}", e.what())
                    throw;
                }
            }
        }

        void TableModifier::deleteInTableWithBookIdBind(int bookId, SQLite::Statement& query)
        {
            query.bind(":book_id", bookId);
            query.exec();
        }

        int TableModifier::modifyBookTable(std::shared_ptr<BookManager::Book::Abstraction::Book> bookToAdd, SQLite::Statement& query)
        {
            query.bind(":type", static_cast<int>(bookToAdd->getType()));
            query.bind(":title", bookToAdd->title);
            bindPointersType<BookManager::Category::Category, const int>(
                query, ":main_category", bookToAdd->mainCategory, &BookManager::Category::Category::getId);
            bindPointersType<BookManager::Entity::Publisher, const int>(
                query, ":publisher", bookToAdd->publisher, &BookManager::Entity::Publisher::getId);
            bindPointersType<BookManager::Entity::BookSerie, const int>(
                query, ":book_serie", bookToAdd->bookSerie, &BookManager::Entity::BookSerie::getId);
            bindOptionalDate(query, ":published_date", bookToAdd->published);
            bindOptionalDate(query, ":purchased_date", bookToAdd->purchasedDate);
            if(bookToAdd->price)
                query.bind(":price", bookToAdd->price.value()); // YYY-MM-DD
            else
                query.bind(":price"); // bind to null

            query.bind(":status", static_cast<int>(bookToAdd->status));
            query.bind(":is_read", bookToAdd->isRead);
            bindOptionalDate(query, ":start_reading_date", bookToAdd->startReadingDate);
            bindOptionalDate(query, ":end_reading_date", bookToAdd->endReadingDate);
            query.bind(":rate", bookToAdd->rate);

            try
            {
                auto rowModified = query.exec();
                return rowModified;
            }
            catch(const std::exception& e)
            {
                throw;
            }
        }
    } // namespace Manager
} // namespace BookManager