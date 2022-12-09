#include <MenuTree.hpp>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
  //  QCoreApplication a(argc, argv);
  HackySolutions::MenuTree mtree;
  {
    try {
      auto res = HackySolutions::MenuTree::parseTextAsMT(
          "(2,1),(3,1),(4,1),(5,2),(6,2),(2,false),(7,3)");
      qDebug() << "verboseAll: ";
      res->verboseAll();
      qDebug() << "verboseSubTree(2): ";
      res->verboseSubTree(2);
      qDebug() << "verboseSubTree(1): ";
      res->verboseSubTree(1);
    } catch (std::runtime_error const &re) {
      qDebug() << "Error on parsing -> " << re.what();
    }
  }
  qDebug() << "\n";
  {
    try {
      auto res = HackySolutions::MenuTree::parseTextAsMT(
          "wq321/"  // if enable "ultra_check_format_file" , we get error here
                    // too
          "(2,1),(3,1),(4,1),(5,2),(6,2),(5,false),(2,FALSE),(7,3),(2020,2),("
          "2000,5)");
      qDebug() << "verboseAll: ";
      res->verboseAll();
    } catch (std::runtime_error const &re) {
      qDebug() << "Error on parsing -> " << re.what();
    }
  }
  //  return a.exec();
}
