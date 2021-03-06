#pragma once
#include "BookAbstract/Book.hpp"

namespace BookManager
{
	namespace Book
	{
		struct Novel : public Abstraction::Book
		{
		public:
			Novel() = default;
			Novel(std::string title, std::shared_ptr<Entity::Person> author_) : Book(title, author_) {}
			Novel(std::string title, std::vector<std::shared_ptr<Entity::Person>> author) : Book(title, author) {}
			~Novel() = default;

			BookManager::Book::BookType getType() override { return BookManager:: Book::BookType::Novel; }

		private:


		};
	} // namespace Book
} // namespace BookManager
