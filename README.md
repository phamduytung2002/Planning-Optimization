# Planning Optimization
## MILP modeling guide

- If you need an indicator whether an expression A equals 0, add an auxiliary binary variable z:
>   minimize z
    Mz >= A
    Mz >= -A

- 