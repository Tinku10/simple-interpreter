#pragma once

#include <iostream>

class DataType {
 public:
  DataType();
  virtual ~DataType() = 0;

  virtual std::shared_ptr<DataType> operator+(DataType& other) = 0;
  virtual std::shared_ptr<DataType> operator-(DataType& other) = 0;
  virtual std::shared_ptr<DataType> operator/(DataType& other) = 0;
  virtual std::shared_ptr<DataType> operator*(DataType& other) = 0;
  virtual std::shared_ptr<DataType> operator+() = 0;
  virtual std::shared_ptr<DataType> operator-() = 0;
  virtual std::shared_ptr<DataType> operator=(DataType& other) = 0;
  void error(const std::string& left,
             const std::string& op,
             const std::string& right);

  friend std::ostream& operator<<(std::ostream& cout,
                                  std::shared_ptr<DataType> var);
};

class IntType;
class FloatType;
class StringType;

class IntType : public DataType {
 public:
  int value;
  IntType();
  IntType(int value);
  ~IntType() override;

  std::shared_ptr<DataType> operator+(DataType& other) override;
  std::shared_ptr<DataType> operator-(DataType& other) override;
  std::shared_ptr<DataType> operator/(DataType& other) override;
  std::shared_ptr<DataType> operator*(DataType& other) override;
  std::shared_ptr<DataType> operator+() override;
  std::shared_ptr<DataType> operator-() override;
  std::shared_ptr<DataType> operator=(DataType& other) override;
};

class FloatType : public DataType {
 public:
  float value;
  FloatType();
  FloatType(float value);
  ~FloatType() override;
  std::shared_ptr<DataType> operator+(DataType& other) override;
  std::shared_ptr<DataType> operator-(DataType& other) override;
  std::shared_ptr<DataType> operator/(DataType& other) override;
  std::shared_ptr<DataType> operator*(DataType& other) override;
  std::shared_ptr<DataType> operator+() override;
  std::shared_ptr<DataType> operator-() override;
  std::shared_ptr<DataType> operator=(DataType& other) override;
};

class StringType : public DataType {
 public:
  std::string value;
  StringType();
  StringType(const std::string& value);
  ~StringType() override;
  std::shared_ptr<DataType> operator+(DataType& other) override;
  std::shared_ptr<DataType> operator-(DataType& other) override;
  std::shared_ptr<DataType> operator/(DataType& other) override;
  std::shared_ptr<DataType> operator*(DataType& other) override;
  std::shared_ptr<DataType> operator+() override;
  std::shared_ptr<DataType> operator-() override;
  std::shared_ptr<DataType> operator=(DataType& other) override;
};
