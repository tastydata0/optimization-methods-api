# Введение

optimization-methods-api - API с методами оптимизации. На данный момент имеется функция поиска минимума/максимума функции на некотором отрезке.

## Использование

Используется, когда нужно найти минимум/максимум функции на некотором отрезке.

## Авторизация

Все запросы API требуют использования сгенерированного ключа API.

Для аутентификации API запроса нужно указать свой ключ API в заголовке «api_key».

Кроме того, вы можете добавить `api_key=[API_KEY]` в качестве параметра GET, чтобы авторизоваться в API. Но обратите внимание, что это может оставить следы в истории, если доступ к API происходит через браузер.
```http
GET /dichotomy/?api_key=12345678901234567890123456789012
```

| Параметр | Тип | Описание |
| :--- | :--- | :--- |
| `api_key` | `string` | **Обязателен**. Ваш API ключ |
| `left_bound` | `float` |  **Обязателен**. Левая граница поиска |
| `right_bound` | `float` |  **Обязателен**. Правая граница поиска |
| `function` | `string` |  **Обязателен**. Исследуемая функция на Python |
| `e` | `float` |  **Обязателен**. Точность |
| `l0` | `float` |  **Обязателен**. Точность |
| `target` | `string` |  **Обязателен**. Цель поиска. Может принимать одно из 2 значений: `min` или `max` |

## Ответ

Многие конечные точки API возвращают JSON-представление созданных или отредактированных ресурсов. Однако, если отправлен неверный запрос или произошла какая-либо другая ошибка, optimization-methods-api возвращает ответ JSON в следующем формате:

Штатная ситуация:
```javascript
{
    "f*": 153360.24721774017,
    "n": 14,
    "success": true,
    "x*": 9.8921875
}
```

Произошла ошибка:
```javascript
{
    "success": false,
    "error": "Missing parameter left_bound"
}
```

Атрибут `success` описывает, была ли операция успешной.
В случае ошибки, данные о ней будут находиться в поле `error`.

## Статус коды

optimization-methods-api возвращает следующие статус коды в своем API:

| Статус код | Описание |
| :--- | :--- |
| 200 | `OK` |
| 400 | `BAD REQUEST` |
| 403 | `FORBIDDEN` |
| 404 | `NOT FOUND` |
| 500 | `INTERNAL SERVER ERROR` |
