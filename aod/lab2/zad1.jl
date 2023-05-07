using JuMP, GLPK, Test

m = Model(GLPK.Optimizer)

# DANE
liczba_lotnisk = 4
liczba_firm = 3
lotniska = ["Lotnisko 1", "Lotnisko 2","Lotnisko 3","Lotnisko 4"]
dostawcy = ["Firma 1", "Firma 2", "Firma 3"]
podaż = Dict(zip(dostawcy, [275000, 550000, 660000]))
popyt = Dict(zip(lotniska, [110000, 220000, 330000, 440000]))
koszta = [10 10 9 11; 7 11 12 13; 8 14 4 9]

# Zmienne - ilości paliwa dostarczane na lotniska przez firmy
@variable(m, w[dostawcy, lotniska] >= 0)
println(size(w))
println(size(koszta))
# Funkcja celu - minimalizacja kosztów
@objective(m, Min, sum(koszta[i,j]*w[dostawcy[i],lotniska[j]] for i in 1:3, j in 1:4))

# Ograniczenia - sumy paliwa dostarczonego przez każdą firmę na wszystkie lotniska
for i in 1:liczba_firm
    @constraint(m, sum(w[dostawcy[i], lotniska[j]] for j in 1:liczba_lotnisk) <= podaż[dostawcy[i]])
end

# Ograniczenia - sumy paliwa dostarczonego na poszczególne lotniska
for j in 1:liczba_lotnisk
    @constraint(m, sum(w[dostawcy[i],lotniska[j]] for i in 1:liczba_firm) >= popyt[lotniska[j]])
end

# Rozwiązanie
optimize!(m)

# Wyświetlenie wyniku
println("Koszt minimalny: \$", objective_value(m))
println("Ilości paliwa dostarczone na poszczególne lotniska:")
for j in 1:liczba_lotnisk
    println("Lotnisko ", j, ":")
    for i in 1:liczba_firm
        println(dostawcy[i], ": ", value(w[dostawcy[i],lotniska[j]]), " galonów")
    end
end

