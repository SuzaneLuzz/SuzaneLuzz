# 📚 Library Manager (C++)

Sistema de gerenciamento de biblioteca em linha de comando, desenvolvido em **C++17**. Projeto de nível intermediário criado para portfólio, demonstrando orientação a objetos, uso da STL, persistência em arquivos e tratamento de exceções.

## ✨ Funcionalidades

- Cadastro, remoção e listagem de **livros** (com ordenação por título ou ano)
- Busca de livros por título ou autor (case-insensitive)
- Cadastro e remoção de **membros**
- Registro de **empréstimos** com data de vencimento automática (14 dias)
- Devolução de livros, com identificação de empréstimos **atrasados**
- **Persistência em disco**: os dados são salvos em arquivos de texto e recarregados a cada execução
- Tratamento de erros com hierarquia própria de exceções (`LibraryException`, `BookNotFoundException`, `BookNotAvailableException`, etc.)

## 🧱 Conceitos de C++ aplicados

- Orientação a objetos (encapsulamento, composição entre `Library`, `Book`, `Member` e `Loan`)
- STL: `std::vector`, algoritmos (`std::sort`, `std::find_if`, `std::copy_if`), lambdas
- Sobrecarga de operadores (`Date`)
- RAII e `std::filesystem` para manipulação de diretórios/arquivos
- Exceções customizadas derivadas de `std::runtime_error`
- Separação em módulos (`include/` para headers, `src/` para implementação)
- Testes automatizados independentes de framework externo

## 📁 Estrutura do projeto

```
cpp-library-manager/
├── include/          # Headers (.hpp) das classes do domínio
├── src/              # Implementação (.cpp) + main.cpp (CLI)
├── tests/            # Testes automatizados (assert-based)
├── data/             # Arquivos de persistência (gerados em tempo de execução)
├── CMakeLists.txt    # Build com CMake
└── Makefile          # Build alternativo com make/g++
```

## 🚀 Como compilar e executar

### Opção 1: CMake

```bash
cmake -S . -B build
cmake --build build
./build/library_manager
```

### Opção 2: Makefile

```bash
make run
```

## ✅ Como rodar os testes

```bash
# Com CMake
cmake --build build --target library_tests
ctest --test-dir build

# Ou com make
make test
```

## 💾 Persistência de dados

Ao sair do programa (opção `0` do menu), os dados são salvos em `data/books.txt`, `data/members.txt` e `data/loans.txt` no formato delimitado por `|`. Esses arquivos são recarregados automaticamente na próxima execução.

## 🗺️ Possíveis extensões futuras

- Cálculo de multas por atraso na devolução
- Interface gráfica (Qt) ou web (via API REST)
- Migração da persistência para SQLite

---

Desenvolvido como parte do portfólio de programação de **Suzane Luz**.
