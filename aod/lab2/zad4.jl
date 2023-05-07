using JuMP, GLPK, LinearAlgebra

model = Model(GLPK.Optimizer)

k = 3
m = 5
n = 6
containers = [ 
    (1, 1),
    (2, 4),
    (3, 3),
    (4, 4),
    (5, 5),
    (6,1)
 ]


@variable(model, x[1:n, 1:m], Bin)


@objective(model, Min, sum(x))

for c in containers
    c1 = c[1]
    c2 = c[2]
    @constraint(model, x[c1, c2] == 0)

    
@constraint(model, sum(x[c1+q,c2]  for q in -min(c1-1, k-1):-1) + sum(x[c1,c2+q]  for q in -min(c2-1, k-1):-1) + sum(x[c1+q,c2]   for q in 1:min(k-1, n-c1)) + sum(x[c1,c2+q]   for q in 1:min(k-1, m-c2)) >=1) 
    
end

optimize!(model)
println("minimalna liczba kamer: ", objective_value(model))
println("rozmieszczenie kamer:")
for i in 1:n
    for j in 1:m
        print(value(x[i,j]), " ")
    end
    print("\n")
end

