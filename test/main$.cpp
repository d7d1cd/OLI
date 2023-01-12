#include "GOOGLE/gtest.h"


std::string TEST_LIBRARY;


int main(int argc, char** argv)
{
  // Добавлю тестовую библиотеку в либл
  TEST_LIBRARY = strtok(argv[0], "/");
  auto addlible = system(("ADDLIBLE " + TEST_LIBRARY).c_str());

  // Запускаю тесты
  testing::InitGoogleTest(&argc, argv);
  auto result = RUN_ALL_TESTS();

  // Если библиотека была добавлена в либл, удалю ее оттуда
  if (!addlible)
    system(("RMVLIBLE " + TEST_LIBRARY).c_str());

  return result;
}

