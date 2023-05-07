using JuMP, GLPK, Test

m = Model(GLPK.Optimizer)

# DANE
liczba_produktów = 4
liczba_maszyn = 3

koszt_masz = [2,2,3]/60
zysk_prod = [5,6,5,4]
popyt = [400, 100, 150, 500]
produkty = ["P1", "P2", "P3", "P4"]
maszyny = ["m1", "M2", "M3"]

czasy = [5 3 4 4; 10 6 5 2; 6 4 3 1]

@variable(m, w[produkty] >= 0)

@objective(m, Max, sum(w[produkty[i]] * (zysk_prod[i] - sum(koszt_masz[j] * czasy[j, i] for j in 1:liczba_maszyn)) for i in 1:liczba_produktów))

for i in 1:liczba_produktów
    @constraint(m, w[produkty[i]] <= popyt[i])
    @constraint(m, w[produkty[i]] >= 0)

end

for i in 1:liczba_maszyn
    @constraint(m, sum(czasy[i, j] * w[produkty[j]] for j in 1:liczba_produktów) <= 3600)
    @constraint(m, sum(czasy[i, j] * w[produkty[j]] for j in 1:liczba_produktów) >= 0)

end

# Rozwiązanie
optimize!(m)

println("Maksymalny zysk: \$", objective_value(m))
println("Kilogramy wyprodykowanych produktów: ")
for i in 1:liczba_produktów
    println(produkty[i], ": ", value(w[produkty[i]]), " kg")
end