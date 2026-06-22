# TODO — Project Completion Plan Against Requirements

## Compliance Check Summary

Based on the provided requirement sheet, the project is **not fully in accordance** yet.

### Functional requirements status
- ✅ User registration and authentication
- ⚠️ Searching/filtering books by title, author, category (**title only is wired in UI; author/category filtering incomplete**)
- ✅ Borrowing and returning books
- ✅ Reserving unavailable books
- ⚠️ Tracking borrowing history (**transactions are stored, but no user-facing history flow/use case output**)
- ❌ Calculating penalties for overdue returns (**declared and used, but not implemented; build currently fails**) 
- ⚠️ Managing inventory add/update/remove (**add/remove exist; update missing**)
- ✅ Support for physical books and digital books

### Actors/use cases alignment
- Reader, Librarian, Administrator roles exist.
- Use cases 1–5, 7, 8 are partially/mostly represented.
- Use case 6 (Calculate penalties) is incomplete due to missing implementation.

## Completion Plan

1. **Stabilize build for penalty feature**
   - Implement `calculateOverduePenalties` in `TransactionManager` to match the existing declaration and menu flow.
   - Ensure overdue logic only targets currently borrowed items past due date.

2. **Complete search/filter requirement coverage**
   - Add search/filter by author and category in `InventoryManager`.
   - Update reader/librarian search flow to let users choose filter mode (title/author/category/all).

3. **Add borrowing history use case output**
   - Add query methods in `TransactionManager` to retrieve history by user (and optionally by book).
   - Add a menu option for readers (and optionally librarians/admin) to view borrowing history.

4. **Complete inventory management requirement (update)**
   - Add an update-book capability (title/author/category/availability and type-specific metadata where valid).
   - Expose update action in librarian (and optionally admin) menu.

5. **Harden role/use-case mapping**
   - Ensure each required use case is clearly accessible from the intended actor menu.
   - Replace placeholder admin inventory actions with real, connected flows or remove misleading placeholders.

6. **Validation and acceptance pass**
   - Rebuild project and verify no linker/compiler errors.
   - Run manual scenario checks for each listed functional requirement and use case.
   - Confirm persistence correctness in `accounts.txt`, `books.txt`, and `transactions.txt`.

## Suggested Done Criteria
- All 8 functional requirements demonstrably available from the UI.
- All 8 listed use cases executable end-to-end.
- Project compiles cleanly from source without unresolved symbols.
