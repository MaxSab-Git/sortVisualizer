sv.setTimeMultiplier(20.0 / (sv.getSize() ^ 2))

for i = 2, sv.getSize(), 1 do
    for j = 1, sv.getSize(), 1 do
        local success, result = sv.compare(i, j)
        if not success then return end
        if result then
            sv.swap(i, j)
        end
    end
end