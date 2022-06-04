#include "DataType.h"

#include <iomanip>

DataType::DataType() {
}

void DataType::error(const std::string& left,
                     const std::string& op,
                     const std::string& right) {
  std::string msg = "cannot perform " + left + " " + op + " " + right;
  throw std::invalid_argument(msg);
}

DataType::~DataType() {
}

std::ostream& operator<<(std::ostream& cout, std::shared_ptr<DataType> var) {
  std::shared_ptr<IntType> right = std::dynamic_pointer_cast<IntType>(var);
  if (right == nullptr) {
    std::shared_ptr<FloatType> right
        = std::dynamic_pointer_cast<FloatType>(var);
    if (right == nullptr) {
      std::shared_ptr<StringType> right
          = std::dynamic_pointer_cast<StringType>(var);
      if (right == nullptr) {
        std::shared_ptr<BooleanType> right
            = std::dynamic_pointer_cast<BooleanType>(var);
        if (right == nullptr) {
          throw std::invalid_argument("unknown type to print");
        }
        cout << right->value;
        return cout;
      }
      cout << right->value;
      return cout;
    }
    cout << std::fixed << std::setprecision(2) << right->value;
    return cout;
  }
  cout << right->value;
  return cout;
}

IntType::IntType() {
}
IntType::IntType(int value) : value(value) {
}
IntType::~IntType() {
}

FloatType::FloatType() {
}
FloatType::FloatType(float value) : value(value) {
}
FloatType::~FloatType() {
}

BooleanType::BooleanType() {
}
BooleanType::BooleanType(bool value) : value(value) {
}
BooleanType::~BooleanType() {
}

StringType::StringType() {
}
StringType::StringType(const std::string& value) : value(value) {
}
StringType::~StringType() {
}

std::shared_ptr<DataType> IntType::operator+(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
      std::invalid_argument("not allowed");
    }

    return std::make_shared<FloatType>(FloatType(this->value + right->value));
  }
  return std::make_shared<IntType>(IntType(this->value + right->value));
}

std::shared_ptr<DataType> IntType::operator-(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }

    return std::make_shared<FloatType>(FloatType(this->value - right->value));
  }
  return std::make_shared<IntType>(IntType(this->value - right->value));
}

std::shared_ptr<DataType> IntType::operator*(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }

    return std::make_shared<FloatType>(FloatType(this->value * right->value));
  }
  return std::make_shared<IntType>(IntType(this->value * right->value));
}

std::shared_ptr<DataType> IntType::operator/(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }

    return std::make_shared<FloatType>(FloatType(this->value / right->value));
  }
  return std::make_shared<IntType>(IntType(this->value / right->value));
}

std::shared_ptr<DataType> IntType::operator>(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value > right->value);
}

std::shared_ptr<DataType> IntType::operator<(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value < right->value);
}

std::shared_ptr<DataType> IntType::operator>=(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value >= right->value);
}

std::shared_ptr<DataType> IntType::operator<=(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value <= right->value);
}

std::shared_ptr<DataType> IntType::operator!=(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value != right->value);
}

std::shared_ptr<DataType> IntType::operator==(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value == right->value);
}

std::shared_ptr<DataType> IntType::operator+() {
  return std::make_shared<IntType>(IntType(this->value));
}

std::shared_ptr<DataType> IntType::operator-() {
  return std::make_shared<IntType>(IntType(-this->value));
}

std::shared_ptr<DataType> IntType::operator=(DataType& other) {
  std::cout << "Called--------------------------\n";
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }

    return std::make_shared<IntType>(IntType((int)right->value));
  }
  return std::make_shared<IntType>(IntType((int)right->value));
}

std::shared_ptr<DataType> FloatType::operator+(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }
    return std::make_shared<FloatType>(FloatType(this->value + right->value));
  }
  return std::make_shared<FloatType>(FloatType(this->value + right->value));
}

std::shared_ptr<DataType> FloatType::operator-(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }
  }
  return std::make_shared<FloatType>(FloatType(this->value - right->value));
}

std::shared_ptr<DataType> FloatType::operator*(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }
  }
  return std::make_shared<FloatType>(FloatType(this->value * right->value));
}

std::shared_ptr<DataType> FloatType::operator/(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }
  }
  return std::make_shared<FloatType>(FloatType(this->value / right->value));
}

std::shared_ptr<DataType> FloatType::operator>(DataType& other) {
  FloatType* right = dynamic_cast<FloatType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value > right->value);
}

std::shared_ptr<DataType> FloatType::operator<(DataType& other) {
  FloatType* right = dynamic_cast<FloatType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value < right->value);
}

std::shared_ptr<DataType> FloatType::operator>=(DataType& other) {
  FloatType* right = dynamic_cast<FloatType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value >= right->value);
}

std::shared_ptr<DataType> FloatType::operator<=(DataType& other) {
  FloatType* right = dynamic_cast<FloatType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value <= right->value);
}

std::shared_ptr<DataType> FloatType::operator!=(DataType& other) {
  FloatType* right = dynamic_cast<FloatType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value != right->value);
}

std::shared_ptr<DataType> FloatType::operator==(DataType& other) {
  FloatType* right = dynamic_cast<FloatType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value == right->value);
}

std::shared_ptr<DataType> FloatType::operator+() {
  return std::make_shared<FloatType>(FloatType(this->value));
}

std::shared_ptr<DataType> FloatType::operator-() {
  return std::make_shared<FloatType>(FloatType(-this->value));
}

std::shared_ptr<DataType> FloatType::operator=(DataType& other) {
  IntType* right = dynamic_cast<IntType*>(&other);
  if (right == nullptr) {
    FloatType* right = dynamic_cast<FloatType*>(&other);
    if (right == nullptr) {
    }

    return std::make_shared<FloatType>(FloatType(right->value));
  }
  return std::make_shared<FloatType>(FloatType((float)right->value));
}


std::shared_ptr<DataType> BooleanType::operator+(DataType& other) {
}

std::shared_ptr<DataType> BooleanType::operator-(DataType& other) {
}

std::shared_ptr<DataType> BooleanType::operator*(DataType& other) {
}

std::shared_ptr<DataType> BooleanType::operator/(DataType& other) {
}

std::shared_ptr<DataType> BooleanType::operator+() {
}

std::shared_ptr<DataType> BooleanType::operator-() {
}

std::shared_ptr<DataType> BooleanType::operator>(DataType& other) {
  BooleanType* right = dynamic_cast<BooleanType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value > right->value);
}

std::shared_ptr<DataType> BooleanType::operator<(DataType& other) {
  BooleanType* right = dynamic_cast<BooleanType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value < right->value);
}

std::shared_ptr<DataType> BooleanType::operator>=(DataType& other) {
  BooleanType* right = dynamic_cast<BooleanType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value >= right->value);
}

std::shared_ptr<DataType> BooleanType::operator<=(DataType& other) {
  BooleanType* right = dynamic_cast<BooleanType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value <= right->value);
}

std::shared_ptr<DataType> BooleanType::operator!=(DataType& other) {
  BooleanType* right = dynamic_cast<BooleanType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value != right->value);
}

std::shared_ptr<DataType> BooleanType::operator==(DataType& other) {
  BooleanType* right = dynamic_cast<BooleanType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<BooleanType>(this->value == right->value);
}
std::shared_ptr<DataType> BooleanType::operator=(DataType& other) {
  BooleanType* right = dynamic_cast<BooleanType*>(&other);
  if (right == nullptr) {
  }

  return std::make_shared<BooleanType>(BooleanType(right->value));
}

std::shared_ptr<DataType> StringType::operator+(DataType& other) {
  StringType* right = dynamic_cast<StringType*>(&other);
  if (right == nullptr) {
  }
  return std::make_shared<StringType>(StringType(this->value + right->value));
}

std::shared_ptr<DataType> StringType::operator-(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator*(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator/(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator>(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator<(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator>=(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator<=(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator!=(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator==(DataType& other) {
}

std::shared_ptr<DataType> StringType::operator+() {
}

std::shared_ptr<DataType> StringType::operator-() {
}

std::shared_ptr<DataType> StringType::operator=(DataType& other) {
  StringType* right = dynamic_cast<StringType*>(&other);
  if (right == nullptr) {
  }

  return std::make_shared<StringType>(StringType(right->value));
}
