#include "View/PersonPage.hpp"
#include "Utils/Exceptions.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>
#include <QGroupBox>
#include <iostream>

PersonPage::PersonPage(std::shared_ptr<BookManager::Manager::DatabaseManager> databaseManager, QWidget *parent)
    : databaseManager(databaseManager), QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    right =  new QLabel("Something here later");
    initLeftPane();
    layout->addLayout(leftLayout, 50);
    layout->addWidget(right, 50);

    setLayout(layout);
}

void PersonPage::initLeftPane()
{
    leftLayout = new QVBoxLayout();

    initPersonList();

    leftLayout->addLayout(initLeftPaneHeader());

    leftLayout->addWidget(initFilterButton());
    leftLayout->addWidget(personTableView);
}

void PersonPage::initPersonList()
{
    auto personVector = databaseManager->getPersonVector(20, 0, BookManager::Entity::Role::Undefined);
    QList<BookManager::Entity::Person> personList(personVector.begin(), personVector.end());

    personModel = new PersonModel(personList);
    personTableView = new QTableView();

    personTableView->setModel(personModel);
    personTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    personTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    personTableView->verticalHeader()->hide();
    personTableView->setSortingEnabled(true);
    personTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QHBoxLayout* PersonPage::initLeftPaneHeader()
{
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* headerName = new QLabel("Author/Illustrator");
    QFont fontLabel;
    fontLabel.setPointSize(16);
    headerName->setFont(fontLabel);

    QPushButton* buttonDeletePerson = new QPushButton();
    buttonDeletePerson->setIcon(QIcon(":/icons/trash.png"));
    buttonDeletePerson->setFixedSize(28, 28);
    buttonDeletePerson->setIconSize(QSize(20, 20));
    QObject::connect(buttonDeletePerson, &QPushButton::clicked, this, &PersonPage::deletePersonsSelected);

    headerLayout->addWidget(headerName);
    headerLayout->addWidget(buttonDeletePerson);
    return headerLayout;
}

QGroupBox* PersonPage::initFilterButton()
{
    QGroupBox* filterGroup = new QGroupBox("Filter");

    QHBoxLayout* filterButtonLayout = new QHBoxLayout();
    buttonAllFilter = new QRadioButton("All");
    buttonAllFilter->setChecked(true);
    buttonAuthorFilter = new QRadioButton("Author");
    buttonIllustratorFilter = new QRadioButton("Illustrator");
    filterButtonLayout->addWidget(buttonAllFilter);
    filterButtonLayout->addWidget(buttonAuthorFilter);
    filterButtonLayout->addWidget(buttonIllustratorFilter);

    QObject::connect(buttonAllFilter, &QRadioButton::clicked, this, &PersonPage::setFilter);
    QObject::connect(buttonAuthorFilter, &QRadioButton::clicked, this, &PersonPage::setFilter);
    QObject::connect(buttonIllustratorFilter, &QRadioButton::clicked, this, &PersonPage::setFilter);

    filterGroup->setLayout(filterButtonLayout);
    return filterGroup;
}

void PersonPage::deletePersonsSelected()
{
    QItemSelectionModel* selection = personTableView->selectionModel();
    QModelIndexList listIndexes = selection->selectedRows();
    std::sort(listIndexes.rbegin(), listIndexes.rend()); // reverse iterator, greater to lower
    for(auto& index : listIndexes)
    {
        deletePersonSelected(index);
    }

}

void PersonPage::deletePersonSelected(const QModelIndex& index)
{
    auto personSelected = personModel->data(index, PersonModel::PersonModel::PersonObject).value<BookManager::Entity::Person>();
    try
    {
        databaseManager->deletePerson(personSelected.getId(), false);
        personModel->removeRow(index.row());
        return;
    }
    catch(const Utils::Exceptions::EC_ForeignKeyFound& e)
    {
        auto response = QMessageBox::critical(this, "Error deleting an Author" , e.getStr(), QMessageBox::Yes | QMessageBox::No);
        if(response == QMessageBox::Yes)
        {
            e.executeCallback();
            personModel->removeRow(index.row());
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

std::vector<BookManager::Entity::Person> PersonPage::getPersonFromDatabase()
{
    if (buttonAuthorFilter->isChecked())
        return databaseManager->getPersonVector(20, 0, BookManager::Entity::Role::Author);
    else if (buttonIllustratorFilter->isChecked())
        return databaseManager->getPersonVector(20, 0, BookManager::Entity::Role::Illustrator);
    else
        return databaseManager->getPersonVector(20, 0, BookManager::Entity::Role::Undefined);
}

void PersonPage::setFilter()
{
    auto personVector = getPersonFromDatabase();
    QList<BookManager::Entity::Person> personList(personVector.begin(), personVector.end());
    personModel = new PersonModel(personList);
    personTableView->setModel(personModel);
}