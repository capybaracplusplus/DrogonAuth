# DrogonAuth

## Описание

Этот проект реализует базовую систему аутентификации и авторизации с использованием JWT (JSON Web Token) на фреймворке Drogon. Он предоставляет следующие функции:
- Регистрация пользователей
- Аутентификация с получением токена
- Выход из системы

## Зависимости

Для работы проекта необходимы следующие библиотеки и фреймворки:

- **[Drogon](https://github.com/drogonframework/drogon)**: высокопроизводительный фреймворк для разработки веб-приложений на C++. Drogon предлагает быстрый и масштабируемый способ создания RESTful API.

- **[JWT-CPP](https://github.com/arun11299/cpp-jwt)**: библиотека для работы с JWT (JSON Web Tokens) в C++. Эта библиотека используется для создания, верификации и декодирования JWT токенов.

- **[redis-plus-plus](https://github.com/sewenew/redis-plus-plus)**: C++ клиент для работы с Redis. Используется для хранения сессий и кэширования данных.

- **[Bcrypt.cpp](https://github.com/hilch/Bcrypt.cpp)**: C++ библиотека для хеширования паролей с использованием алгоритма bcrypt. Эта библиотека применяется для безопасного хранения паролей.

## Примеры API

### Регистрация
**POST /sign-up**  
Тело запроса:
```json
{
  "username": "example",
  "email": "example@gmail.com",
  "password": "123456789&mM"
}
```
Тело ответа:
```json
{
  "message": "User registered successfully"
}
```

### Аутентификация
**POST /sign-in**  
Тело запроса:
```json
{
  "username": "example",
  "email": "example@gmail.com",
  "password": "123456789&mM"
}
```
Тело ответа:
```json
{
  "accessToken": "eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE3MzY5MDk2MTcsImlhdCI6MTczNjgwMTYxNywiaXNzIjoiQ2FweSIsInN1YiI6IjM2In0.y-2Hv8ES-M9FUyWj8W2iy9yrTSKQfISaKdLnuzV0OMk",
  "message": "The user has successfully logged into the account",
  "userId": 1
}
```
![упс..](./pic/authRefreshToken.png)

### Изменение пароля
**POST /changePassword**  
Тело запроса:
```json
{
  "accessToken": "eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE3MzY5NjEzNzgsImlhdCI6MTczNjg1MzM3OCwiaXNzIjoiQ2FweSIsInN1YiI6IjMifQ.p2OtD-GCZBizt_bHv5IOPKRwcajMxFoaftOWSeOxDRU",
  "password": "New123456789&mM"
}
```
Тело ответа:
```json
{
  "message": "New password set successfully"
}
```

### Обновление access token
**POST /getNewAccessToken**  
Тело запроса:
```json
{
  "accessToken": "eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE3MzY5Mzg1NzgsImlhdCI6MTczNjgzMDU3OCwiaXNzIjoiQ2FweSIsInN1YiI6IjQwIn0.wxL6djVoY-0uBt1XcaEG3DwPe-vQ1-6yGSgiFyDuaLQ"
}
```
Тело ответа:
```json
{
  "accessToken": "eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE3MzY5Mzg1NzgsImlhdCI6MTczNjgzMDU3OCwiaXNzIjoiQ2FweSIsInN1YiI6IjQwIn0.wxL6djVoY-0uBt1XcaEG3DwPe-vQ1-6yGSgiFyDuaLQ",
  "message": "The user has successfully updated the access token",
  "userId": 1
}
```

### Выход из системы
**POST /logout**  
Тело запроса:
```json
{
  "username": "example",
  "email": "example@gmail.com",
  "password": "123456789&mM"
}
```
Тело ответа:
```json
{
  "logout": "ok",
  "message": "The user has successfully logged out of the account."
}
```
