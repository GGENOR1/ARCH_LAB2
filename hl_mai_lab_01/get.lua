
function base64_encode(data)
    local b = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
    return ((data:gsub('.', function(x)
        local r, b = '', x:byte()
        for i = 8, 1, -1 do
            r = r .. (b % 2 ^ i - b % 2 ^ (i - 1) > 0 and '1' or '0')
        end
        return r
    end) .. '0000'):gsub('%d%d%d?%d?%d?%d?', function(x)
        if #x < 6 then return '' end
        local c = 0
        for i = 1, 6 do
            c = c + (x:sub(i, i) == '1' and 2 ^ (6 - i) or 0)
        end
        return b:sub(c + 1, c + 1)
    end) .. ({ '', '==', '=' })[#data % 3 + 1])
end



local id_min = 1 
local id_max = 10 


local number = math.random(id_min, id_max)

local body = ''

local headers = {}
headers["Content-Type"] = "application/json"

local username = "admin"  
local password = "admin"  
local auth = username .. ":" .. password
local auth_base64 = base64_encode(auth)

function order_request()
    local number = math.random(id_min, id_max)
    local body = ''
    local headers = {}
    headers["Content-Type"] = "application/json"
    headers["Authorization"] = "Basic " .. auth_base64
    return wrk.format("GET", "/order?id=" .. tostring(number), headers, body)
end

function conf_request()
    local number = math.random(id_min, id_max)
    local body = ''
    local headers = {}
    headers["Content-Type"] = "application/json"
    headers["Authorization"] = "Basic " .. auth_base64
    return wrk.format("GET", "/conference?id=" .. tostring(number), headers, body)
end

function all_order_request()
    local body = ''
    local headers = {}
    headers["Authorization"] = "Basic " .. auth_base64
    headers["Content-Type"] = "application/json"
    return wrk.format("GET", "/report", headers, body)
end

function all_order_request()
    local body = ''
    local headers = {}
    headers["Authorization"] = "Basic " .. auth_base64
    headers["Content-Type"] = "application/json"
    return wrk.format("GET", "/report", headers, body)
end
request = function()
    local requests = {order_request} 
    local chosen_request = requests[math.random(1, #requests)]
    return chosen_request()
end