using JuMP, GLPK

m = Model(GLPK.Optimizer)

# Dane
ilo_paliwa = [110000, 220000, 330000, 440000]
dostawcy = ["Firma 1", "Firma 2", "Firma 3"]
koszt = [10 7 8; 10 11 14; 9 12 4; 11 13 9]
dostepnosc = Dict(zip(dostawcy, [275000, 550000, 660000]))

# Zmienne - ilości paliwa dostarczone na poszczególne lotniska przez każdą firmę
@variable(m, x[dostawcy, 1:4] >= 0)
println(size(x))
# Funkcja celu - minimalizacja kosztów
@objective(m, Min, sum(koszt[i,j]*x[dostawcy[i],j] for i in 1:3, j in 1:4))

# Ograniczenia - sumy paliwa dostarczonego przez każdą firmę na wszystkie lotniska
for i in 1:3
    @constraint(m, sum(x[dostawcy[i],j] for j in 1:4) <= dostepnosc[dostawcy[i]])
end

# Ograniczenia - sumy paliwa dostarczonego na poszczególne lotniska
for j in 1:4
    @constraint(m, sum(x[dostawcy[i],j] for i in 1:3) == ilo_paliwa[j])
end

# Rozwiązanie
optimize!(m)

# Wyświetlenie wyniku
println("Koszt minimalny: \$", objective_value(m))
println("Ilości paliwa dostarczone na poszczególne lotniska:")
for j in 1:4
    println("Lotnisko ", j, ":")
    for i in 1:3
        println(dostawcy[i], ": ", value(x[dostawcy[i],j]), " galonów")
    end
end
