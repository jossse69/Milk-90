-- Define two points and their velocities
local point1 = {x = 10, y = 10, vx = 1, vy = 1}
local point2 = {x = 40, y = 40, vx = 1, vy = -1}
local screenWidth = 255 -- Assuming this is the screen width
local screenHeight = 170 -- Assuming this is the screen height

function load()
    -- Initialization code can go here
end

function update(deltaTime)
    -- Update positions based on velocity and deltaTime
    point1.x = point1.x + point1.vx * deltaTime
    point1.y = point1.y + point1.vy * deltaTime
    point2.x = point2.x + point2.vx * deltaTime
    point2.y = point2.y + point2.vy * deltaTime

    -- Bounce off the edges for point1
    if point1.x <= 0 or point1.x >= screenWidth then point1.vx = -point1.vx end
    if point1.y <= 0 or point1.y >= screenHeight then point1.vy = -point1.vy end

    -- Bounce off the edges for point2
    if point2.x <= 0 or point2.x >= screenWidth then point2.vx = -point2.vx end
    if point2.y <= 0 or point2.y >= screenHeight then point2.vy = -point2.vy end
end


function draw()
    -- Clear the screen with the first color in the palette
    clearScreen(0)
    -- Draw the two pixels and the line connecting them
    drawLine(point1.x, point1.y, point2.x, point2.y, 2) -- Use another color index
    drawPixel(point1.x, point1.y, 1) -- Use a color index other than 0
    drawPixel(point2.x, point2.y, 1) -- Use a color index other than 0

end

-- Call load to perform any initial setup
load()
