#pragma once
#include "BookAbstract/Book.hpp"

namespace BookManager
{
	namespace Book
	{
		struct Artbook : public Abstraction::Book
		{
		public:
			Artbook() = default;
			Artbook(std::string title, std::shared_ptr<Entity::Person> author_) : Book(title, author_) {}
			Artbook(std::string title, std::vector<std::shared_ptr<Entity::Person>> author) : Book(title, author) {}
			~Artbook() = default;

			BookManager::Book::BookType getType() override { return BookManager:: Book::BookType::ArtBook; }

		private:


		};
	} // namespace Book
} // namespace BookManager