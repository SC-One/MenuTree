#ifndef MENU_TREE_STRUCTURE_H
#define MENU_TREE_STRUCTURE_H
#include <QFile>
#include <QMultiMap>
#include <QString>
#include <QUrl>

namespace Structures {
// T values should be unique!
template <typename T>
class MenuTree {
  template <typename V>
  struct Node {
    operator V() const { return value; }
    bool operator==(V const& otherValue) const { return value == otherValue; }
    bool operator==(Node const& other) const { return value == other.value; }
    bool operator<(Node const& other) const { return value < other.value; }

    V value;
    bool permission;
  };

 public:
  static MenuTree parseFileAsMT(QUrl const& filePath,
                                QString const& KPERMISSION_DENIED = "false",
                                QString const& KPERMISSION_ALLOW = "true");

  static MenuTree parseTextMT(QString const& text,
                              QString const& KPERMISSION_DENIED = "false",
                              QString const& KPERMISSION_ALLOW = "true");

 public:
  void insert(T const val, T const parent, bool permission = true) {
    _mainMap.insert(parent, {val, permission});
  }
  void insert(T const val, bool permission) {
    for (auto const& item : _mainMap.keys()) {
      QList<int> const values = item.values(val);
    }
    _mainMap[val];
    _mainMap.insert(parent, {val, permission});
  }

 private:
  QMultiMap<Node<T>, Node<T>> _mainMap;
};
}  // namespace Structures
#endif  // MENU_TREE_STRUCTURE_H
