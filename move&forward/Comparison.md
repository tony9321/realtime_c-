### std::move vs std::forward

|Function|Meaning|Used when|
|---|---|---|
|`std::move(x)`|Always casts to rvalue (`T&&`), allowing move|When you _know_ the object won’t be used again|
|`std::forward<T>(x)`|Preserves original value category (lvalue/rvalue)|Inside templates (perfect forwarding)|

`std::move` = _“steal this”_  
`std::forward` = _“forward it as it was passed to me”_