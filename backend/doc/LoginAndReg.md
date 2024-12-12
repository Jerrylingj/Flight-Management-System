## 登录
- /api/login
传入
```ts
{
    "telephone":string
    "password":string
}
```
成功返回
```ts
{
    "code":200,
    "message":"操作成功",
    "data":{
        "telephone":string,
        "token":string
    }
}
```
失败返回
```ts
{
    "code":500,
    "message":"失败原因",
    "data":null
}
```

## 注册
- /api/register
传入
```ts
{
    "username":string,
    "telephone":string,
    "password":string
}
```
成功返回
```ts
{
    "code":200,
    "message":"操作成功",
    "data":"注册成功!"
}
```
失败返回
```ts
{
    "code":500,
    "message":"失败原因",
    "data":null
}
```