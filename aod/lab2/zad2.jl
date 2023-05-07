using JuMP, GLPK, Test

m = Model(GLPK.Optimizer)

# DANE
liczba_zmian = 3
liczba_dzielnic = 3

dzielnice = ["Dzielnica 1", "Dzielnica 2", "Dzielnica 3"]
zmiany = ["Zmiana 1", "Zmiana 2", "Zmiana 3"]

low = [2 3 3; 4 6 7; 3 5 6]
upp = [3 5 8; 7 7 12; 5 10 10]
popyt_zmiany = [10, 20, 18]
popyt_dzielnice = [10, 14, 13]
@variable(m, w[zmiany, dzielnice])

for i in 1:liczba_dzielnic
    for j in 1:liczba_zmian
        set_upper_bound(w[zmiany[j], dzielnice[i]], upp[i, j])
        set_lower_bound(w[zmiany[j], dzielnice[i]], low[i, j])
    end
end

# Funkcja celu - minimalizacja kosztów

@objective(m, Min, sum(w[zmiany[i],dzielnice[j]] for i in 1:3, j in 1:3))

for i in 1:liczba_zmian
    @constraint(m, sum(w[zmiany[i], dzielnice[j]] for j in 1:liczba_dzielnic) >= popyt_zmiany[i])
end

for i in 1:liczba_dzielnic
    @constraint(m, sum(w[zmiany[j], dzielnice[i]] for j in 1:liczba_zmian) >= popyt_dzielnice[i])
end

# Rozwiązanie
optimize!(m)

# Wyświetlenie wyniku
println("Minimalna liczba rodiowozów: \$", objective_value(m))
println("Ilości radiowozów dostarczonych na poszczególne dzielnice:")
for j in 1:liczba_dzielnic
    println("dzielnice ", j, ":")
    for i in 1:liczba_zmian
        println(zmiany[i], ": ", value(w[zmiany[i],dzielnice[j]]), " radiowozów")
    end
end