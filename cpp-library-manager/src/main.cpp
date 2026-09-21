#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "Exceptions.hpp"
#include "Library.hpp"

namespace {

void clearInputError() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        clearInputError();
        std::cout << "Entrada invalida, digite um numero.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

void printBook(const Book& book) {
    std::cout << std::left << std::setw(4) << book.getId() << std::setw(30) << book.getTitle()
              << std::setw(20) << book.getAuthor() << std::setw(6) << book.getYear()
              << std::setw(16) << book.getIsbn() << (book.isAvailable() ? "Disponivel" : "Emprestado")
              << '\n';
}

void printBookHeader() {
    std::cout << std::left << std::setw(4) << "ID" << std::setw(30) << "Titulo" << std::setw(20)
              << "Autor" << std::setw(6) << "Ano" << std::setw(16) << "ISBN" << "Status\n";
    std::cout << std::string(80, '-') << '\n';
}

void printBookList(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "Nenhum livro encontrado.\n";
        return;
    }
    printBookHeader();
    for (const auto& book : books) {
        printBook(book);
    }
}

void printMemberList(const std::vector<Member>& members) {
    if (members.empty()) {
        std::cout << "Nenhum membro cadastrado.\n";
        return;
    }
    std::cout << std::left << std::setw(4) << "ID" << std::setw(25) << "Nome" << "E-mail\n";
    std::cout << std::string(60, '-') << '\n';
    for (const auto& member : members) {
        std::cout << std::left << std::setw(4) << member.getId() << std::setw(25) << member.getName()
                  << member.getEmail() << '\n';
    }
}

void printLoanList(const std::vector<Loan>& loans) {
    if (loans.empty()) {
        std::cout << "Nenhum emprestimo encontrado.\n";
        return;
    }
    std::cout << std::left << std::setw(4) << "ID" << std::setw(8) << "Livro" << std::setw(8)
              << "Membro" << std::setw(12) << "Emprestimo" << std::setw(12) << "Vencimento"
              << "Status\n";
    std::cout << std::string(60, '-') << '\n';
    for (const auto& loan : loans) {
        std::string status = loan.isReturned() ? "Devolvido" : (loan.isOverdue(Date::today()) ? "Atrasado" : "Ativo");
        std::cout << std::left << std::setw(4) << loan.getId() << std::setw(8) << loan.getBookId()
                  << std::setw(8) << loan.getMemberId() << std::setw(12) << loan.getLoanDate().toString()
                  << std::setw(12) << loan.getDueDate().toString() << status << '\n';
    }
}

void printMenu() {
    std::cout << "\n=== Sistema de Gerenciamento de Biblioteca ===\n"
              << " 1. Cadastrar livro\n"
              << " 2. Remover livro\n"
              << " 3. Listar livros (ordenado por titulo)\n"
              << " 4. Listar livros (ordenado por ano)\n"
              << " 5. Buscar livro por titulo\n"
              << " 6. Buscar livro por autor\n"
              << " 7. Cadastrar membro\n"
              << " 8. Remover membro\n"
              << " 9. Listar membros\n"
              << "10. Emprestar livro\n"
              << "11. Devolver livro\n"
              << "12. Listar emprestimos ativos\n"
              << "13. Listar emprestimos atrasados\n"
              << " 0. Salvar e sair\n"
              << "Escolha uma opcao: ";
}

} // namespace

int main() {
    Library library;
    library.load();

    std::cout << "Biblioteca carregada: " << library.getBooks().size() << " livro(s), "
              << library.getMembers().size() << " membro(s), " << library.getLoans().size()
              << " emprestimo(s).\n";

    bool running = true;
    while (running) {
        printMenu();
        int option = readInt("");

        try {
            switch (option) {
                case 1: {
                    std::string title = readLine("Titulo: ");
                    std::string author = readLine("Autor: ");
                    int year = readInt("Ano de publicacao: ");
                    std::string isbn = readLine("ISBN: ");
                    Book& book = library.addBook(title, author, year, isbn);
                    std::cout << "Livro cadastrado com id " << book.getId() << ".\n";
                    break;
                }
                case 2: {
                    int id = readInt("ID do livro a remover: ");
                    library.removeBook(id);
                    std::cout << "Livro removido.\n";
                    break;
                }
                case 3:
                    printBookList(library.listBooksSortedByTitle());
                    break;
                case 4:
                    printBookList(library.listBooksSortedByYear());
                    break;
                case 5: {
                    std::string query = readLine("Buscar por titulo: ");
                    printBookList(library.searchByTitle(query));
                    break;
                }
                case 6: {
                    std::string query = readLine("Buscar por autor: ");
                    printBookList(library.searchByAuthor(query));
                    break;
                }
                case 7: {
                    std::string name = readLine("Nome do membro: ");
                    std::string email = readLine("E-mail: ");
                    Member& member = library.addMember(name, email);
                    std::cout << "Membro cadastrado com id " << member.getId() << ".\n";
                    break;
                }
                case 8: {
                    int id = readInt("ID do membro a remover: ");
                    library.removeMember(id);
                    std::cout << "Membro removido.\n";
                    break;
                }
                case 9:
                    printMemberList(library.getMembers());
                    break;
                case 10: {
                    int memberId = readInt("ID do membro: ");
                    int bookId = readInt("ID do livro: ");
                    Loan& loan = library.borrowBook(memberId, bookId);
                    std::cout << "Emprestimo registrado (id " << loan.getId() << "). Vencimento em "
                              << loan.getDueDate().toString() << ".\n";
                    break;
                }
                case 11: {
                    int loanId = readInt("ID do emprestimo: ");
                    library.returnBook(loanId);
                    std::cout << "Devolucao registrada.\n";
                    break;
                }
                case 12:
                    printLoanList(library.listActiveLoans());
                    break;
                case 13:
                    printLoanList(library.listOverdueLoans());
                    break;
                case 0:
                    library.save();
                    std::cout << "Dados salvos. Ate logo!\n";
                    running = false;
                    break;
                default:
                    std::cout << "Opcao invalida.\n";
            }
        } catch (const LibraryException& ex) {
            std::cout << "Erro: " << ex.what() << '\n';
        } catch (const std::exception& ex) {
            std::cout << "Erro inesperado: " << ex.what() << '\n';
        }
    }

    return 0;
}
