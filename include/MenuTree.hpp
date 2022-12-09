#ifndef MENUTREE_STRUCTURE_H
#define MENUTREE_STRUCTURE_H

#include <QDebug>
#include <QHash>
#include <QObject>
#include <QRegExp>
#include <QRegularExpression>
#include <QString>
#include <cassert>
#include <memory>

namespace HackySolutions {
class MenuTreeNode : public QObject {
  Q_OBJECT
 public:
  // hint: parent null is Root
  explicit MenuTreeNode(QObject *parent = nullptr)
      : QObject(parent), _permission(true) {}

  bool operator==(MenuTreeNode const &other) const {
    return other.getValue() == _value;
  }

  void setPermission(bool permission) { _permission = permission; }
  inline bool getPermission() const { return _permission; }
  void setValue(int value) { _value = value; }
  inline int getValue() const { return _value; }

 private:
  bool _permission;
  int _value;
};
class MenuTree : public QObject {
 public:
  static std::unique_ptr<MenuTree> parseTextAsMT(
      QString const &text, QString const &kPermissionDenied = "false") {
    std::unique_ptr<MenuTree> result(new MenuTree);
    auto [validate, error] = std::invoke([&result, &str = std::as_const(text)]()
                                             -> std::tuple<bool, QString> {
      // strill frustrated  warning // \((\d+)(,\d+|(.false))+\)
      QRegularExpression re(
          "\\((?<child>\\d+)(,)(?<parent>(^$)|(\\d+)|(\\bfalse\\b))*"
          "\\)");
#ifdef ultra_check_format_file
      auto remain = str.split(re);
      for (int i = 0; i < remain.size(); ++i) {
        if (auto lower = remain[i].toLower(); lower.contains("false"))
          return {false,
                  "Please edit format. all false strings should be "
                  "lower case"};
        if ("," != remain[i])
          return {false,
                  QString("Error after %1nth pair, incorrect format.\n"
                          "No match found , Please provide correct format "
                          "data , "
                          "example: "
                          "(2,1),(3,1),(4,1),(5,2),(6,2),(2,false),(7,3)\n"
                          "note: false should be lower case")
                      .arg(i + 1)};
      }
#endif
      auto it = re.globalMatch(str);
      while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString child = match.captured("child");
        QString parent = match.captured("parent");
        {
          bool ok = false;
          if (auto parentValue = parent.toInt(&ok); ok) {
            auto value = child.toInt(&ok);
            if (!ok)  // extra check - note: perl regex checked it
              return {false,
                      QString("first item is not number,check: %1").arg(child)};

            try {
              result->insert(value, parentValue);
            } catch (const char *error) {
              return {false, QString("Insertion error on (%1,%2), %3")
                                 .arg(child)
                                 .arg(parent)
                                 .arg(error)};
            } catch (...) {
              assert("Unexpected! check it by developer on test.");
            }
          } else if (auto permissionValue = parent.toLower() == "false") {
            auto value = child.toInt(&ok);
            if (!ok)  // extra check - note: perl regex checked it
              return {false,
                      QString("first item is not number,check: %1").arg(child)};
            try {
              result->limitPermissionOn(value);
            } catch (const char *error) {
              return {false, QString("Insertion error on (%1,%2), %3")
                                 .arg(child)
                                 .arg(parent)
                                 .arg(error)};
            } catch (...) {
              assert("Unexpected! check it by developer on test.");
            }
          } else if (parent.isEmpty())
            return {false, QString("Empty string: %1").arg(child + ",...")};
          else
            return {false,
                    "Please provide correct format data, example :"
                    "(2,1),(3,1),(4,1),(5,2),(6,2),(2,false),(7,3)\n"
                    "note: false should be lower case"};
        }
      }
#ifndef MENUTREE_STRUCTURE_H
      // this is other implementation that has better parsing errors.
      // but has more over head because we should use QRegExp we never
      // use this , because les verbosing error is better than Extra
      // overhead
      QRegExp re(
          "\\((?<child>\\d+)(,)(?<parent>(^$)|(\\d+)|(\\bfalse\\b))*"
          "\\)");
      qDebug() << pairs << re.errorString() << re.isValid();
      // export data while process validation
      int pos = 0, lastPos = pos;
      //          while ((pos = rx.indexIn(str, pos)) != -1) {
      //            if (pos - lastPos > 1)
      //              return {false,
      //                      QString("wrong format , check problem(at
      //                      %1): %2 ...")
      //                          .arg(lastPos)
      //                          .arg(str.mid(lastPos, pos -
      //                          lastPos))};
      //            auto parsedTarget = rx.cap(1);
      //            auto const sepratedTargets = parsedTarget.remove(0,
      //            1)
      //                                             .remove(parsedTarget.size()
      //                                             - 1, 1)
      //                                             .split(",");
      //            {
      //              bool ok = false;
      //              if (auto parentValue =
      //              sepratedTargets[1].toInt(&ok); ok) {
      //                auto value = sepratedTargets[0].toInt(&ok);
      //                if (!ok)
      //                  return {false, QString("first item is not
      //                  number, check: %1")
      //                                     .arg(str.mid(lastPos, pos -
      //                                     lastPos))};
      //                result->insert(value, parentValue);
      //              } else if (auto permissionValue =
      //                             sepratedTargets[1].toLower() ==
      //                             "false") {
      //                auto value = sepratedTargets[0].toInt(&ok);
      //                if (!ok)
      //                  return {false, QString("first item is not
      //                  number, check: %1")
      //                                     .arg(str.mid(lastPos, pos -
      //                                     lastPos))};
      //                result->limitPermissionOn(value);
      //              } else if (sepratedTargets[1].isEmpty())
      //                return {false, QString("Empty string: %1")
      //                                   .arg(str.mid(lastPos, pos -
      //                                   lastPos))};
      //              else
      //                return {false,
      //                        "Please provide correct format data,
      //                        example: "
      //                        "(2,1),(3,1),(4,1),(5,2),(6,2),(2,false),(7,3)\n"
      //                        "note: false should be lower case"};
      //            }

      //            pos += rx.matchedLength();
      //            lastPos = pos;
      //          }

      //          if (pairs.isEmpty())
      //            return {false,
      //                    "Please provide correct format data in file
      //                    , exaple: "
      //                    "(2,1),(3,1),(4,1),(5,2),(6,2),(2,false),(7,3)"
      //                    "note: false should be lower case"};
      if (result->size() == 0 && pos == -1) {
        return {false,
                "No match found , Please provide correct format data , "
                "example: "
                "(2,1),(3,1),(4,1),(5,2),(6,2),(2,false),(7,3)\n"
                "note: false should be lower case"};
      }
#endif
      return {true, "Success."};
    });

    if (!validate) throw std::runtime_error(error.toStdString());
    //    pairs = text.split("),(");
    //    if (!pairs.isEmpty()) {
    //      pairs.first().remove(0, 1);
    //      pairs.last().remove(pairs.last().size() - 1, 1);
    //    }
    //    //    qDebug() << pairs;
    //    for (auto const &item : pairs) {
    //      //      TODO: auto const seprated = ;
    //    }
    return std::move(result);
  }

 public:
  void limitPermissionOn(int value) {
    if (auto const it = _elements.find(value); _elements.constEnd() != it) {
      deniedAll(*it);
    } else {
      throw "Please try to change permission on valid menu(it's not exist in MenuTree).";
    }
  }
  inline int size() const { return _elements.size(); }
  void insert(int value, int parent, bool permission = true) {
    auto const item = _elements.find(value);
    if (_elements.end() != item)
      throw "double insertion occured";
    else {
      auto parentIt = _elements.find(parent);
      MenuTreeNode *parentNode = nullptr;
      if (_elements.constEnd() == parentIt) {
        if (!_elements.isEmpty()) {
          // we have tree , So new parent that is not exist is ugly insert
          throw("Tree is not connected. (inserted ugly parent.)");
        }
        parentNode = new MenuTreeNode();
        parentNode->setValue(parent);
        if (false == permission) {  // won't be executed - because document
                                    // said: root never have false permission
          parentNode->setPermission(permission);
          deniedAll(parentNode);
        }
        _elements.insert(parent, parentNode);
      } else {
        parentNode = *parentIt;
      }
      auto newNode = new MenuTreeNode(parentNode);
      newNode->setValue(value);
      if (!permission) {
        newNode->setPermission(permission);
        deniedAll(newNode);
      }
      _elements.insert(value, newNode);
    }
  }

  void verboseAll() const {
    for (const auto item : _elements.values()) {
      if (nullptr != item) {
        static const QString FORMATTER("*sub-menu %1 is %2.");
        qDebug() << FORMATTER.arg(item->getValue())
                        .arg(item->getPermission() ? "accessed" : "denied");
      }
    }
  }

  void verboseSubTree(int val) const {
    auto const it = _elements.find(val);
    if (_elements.constEnd() == it)
      qDebug() << QString("Can't find node %1 in MenuTree").arg(val);
    else {
      auto childNode = *it;
      verbose(qobject_cast<MenuTreeNode *>(childNode));
    }
  }

 private:
  void verbose(MenuTreeNode const *node) const {
    static const QString FORMATTER("*sub-menu %1 is %2.");
    if (nullptr != node) {
      qDebug() << FORMATTER.arg(node->getValue())
                      .arg(node->getPermission() ? "accessed" : "denied");
      for (auto const item : node->children()) {
        if (auto menuTreeNode = qobject_cast<MenuTreeNode *>(item);
            nullptr != menuTreeNode)
          verbose(menuTreeNode);
      }
    }
  }
  void deniedAll(MenuTreeNode *node) {
    if (nullptr != node) {
      node->setPermission(false);
      auto items = node->children();
      for (auto item : items) deniedAll(qobject_cast<MenuTreeNode *>(item));
    }
  }

 private:
  QHash<int, MenuTreeNode *> _elements;
};
}  // namespace HackySolutions

#endif  // MENUTREE_H
