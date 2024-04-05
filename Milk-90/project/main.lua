-- Variables for player's health
local maxHealth = 5 -- Maximum health
local currentHealth = 5 -- Current health, which can change

-- Position for the first heart
local heartBarX = 10
local heartBarY = 10

function load()
    -- Initialization code here, if needed
end

function update(deltaTime)

    currentHealth = math.max(0, currentHealth - 0.01)

    -- reset the bar once the health is 0
    if currentHealth == 0 then
        currentHealth = maxHealth
    end
end

function draw()
    clearScreen(0) -- Clear the screen with a background color

    -- Draw the heart bar
    for i = 1, maxHealth do

        -- Optionally, draw empty/gray heart to indicate lost health, and as a shadow effect for the hearts
        drawSprite((heartBarX + (i - 1) * 10) + 1, heartBarY + 1, 1, false, false) -- No flipping
        
        if i <= currentHealth then
            -- Draw full heart
            drawSprite(heartBarX + (i - 1) * 10, heartBarY, 0, false, false) -- No flipping
        end

    end
end
