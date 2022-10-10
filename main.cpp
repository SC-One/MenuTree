#include <MenuTree.hpp>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  Structures::MenuTree<int> mtree;
  return a.exec();
}
