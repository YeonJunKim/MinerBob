math.randomseed(1234)

GlobalState = {}

GlobalState["Enter"] = function(elsa)

 -- do nothing

end

GlobalState["Execute"] = function(elsa)

   dice = math.random()

  if dice < 0.1 then
      elsa:GetFSM():ChangeState(VisitBathroom)
  end
end

GlobalState["Exit"] = function(elsa)

 -- do nothing

end

GlobalState["OnMessage"] = function(elsa, msg)

 print ("["..elsa:Name().."]: ".."Hi honey. Let me make you some of mah fine country stew")
 elsa:GetFSM():ChangeState(CookStew)

end



DoHomework = {}

DoHomework["Enter"] = function(elsa)

 -- do nothing

end

DoHomework["Execute"] = function(elsa)

   dice = math.random(0,2)

  if dice == 0 then
      print ("["..elsa:Name().."]: ".."Moppin' the floor")
  elseif dice == 1 then
     print ("["..elsa:Name().."]: ".."Washin' the dishes")
  else
     print ("["..elsa:Name().."]: ".."Makin' the bed")
  end
end

DoHomework["Exit"] = function(elsa)

 -- do nothing

end



VisitBathroom = {}
VisitBathroom["Enter"] = function(elsa)

  print ("["..elsa:Name().."]: ".."Walkin' to the can. Need to powda mah pretty li'lle nose")

end

VisitBathroom["Execute"] = function(elsa)

   print ("["..elsa:Name().."]: ".."Ahhhhhh! Sweet relief!")


   elsa:GetFSM():RevertToPreviousState();
end

VisitBathroom["Exit"] = function(elsa)

   print ("["..elsa:Name().."]: ".."Leavin' the Jon")

end


CookStew = {}
CookStew["Enter"] = function(elsa)

 print ("["..elsa:Name().."]: ".."Putting the stew in the oven")
 MessageDispatcher.Instance():DispatchMessage(1.5, elsa:ID(), elsa:ID(), 1)
 elsa:SetCooking(true)

end

CookStew["Execute"] = function(elsa)

   print ("["..elsa:Name().."]: ".."Fussin' over food")

end

CookStew["Exit"] = function(elsa)

   print ("["..elsa:Name().."]: ".."Puttin' the stew on the table")

end

CookStew["OnMessage"] = function(elsa, msg)

  print ("["..elsa:Name().."]: ".."StewReady! Lets eat")
  MessageDispatcher.Instance():DispatchMessage(0.0, elsa:ID(), 0, 1)
  elsa:SetCooking(false)
  elsa:GetFSM():ChangeState(DoHomework)
end




----------------------- Bob----------------------------------
-------------------------------------------------------------
eat = false

EnterMineAndDigForNugget = {}

EnterMineAndDigForNugget["Enter"] = function(bob)

 print ("["..bob:Name().."]: ".."Walkin' to the goldmine")

end

EnterMineAndDigForNugget["Execute"] = function(bob)

 bob:AddToGoldCarried(1)
 bob:IncreaseFatigue()

 print ("["..bob:Name().."]: ".."Pickin' up a nugget")

 if bob:PocketsFull() then
   bob:GetFSM():ChangeState(VisitBankAndDepositGold)
 end

 if bob:Thirsty() then
   bob:GetFSM():ChangeState(QuenchThirst)
 end

end

EnterMineAndDigForNugget["Exit"] = function(bob)

  print ("["..bob:Name().."]: ".."Ah'm leavin' the goldmine with mah pockets full o' sweet gold")

end



QuenchThirst = {}

QuenchThirst["Enter"] = function(bob)

 print ("["..bob:Name().."]: ".."Boy, ah sure is thusty! Walking to the saloon")

end

QuenchThirst["Execute"] = function(bob)

 if bob:Thirsty() then
  bob:BuyAndDrinkAWhiskey()
  print ("["..bob:Name().."]: ".."That's mighty fine sippin' liquer")
  print ("["..bob:Name().."]: ".."Depositing gold. Total savings now: "..bob:Wealth())
  bob:GetFSM():ChangeState(EnterMineAndDigForNugget)
 else
  print ("["..bob:Name().."]: ".."ERROR ERROR ERROR!")
 end
end

QuenchThirst["Exit"] = function(bob)

  print ("["..bob:Name().."]: ".."Leaving the saloon, feelin' good")

end




VisitBankAndDepositGold = {}

VisitBankAndDepositGold["Enter"] = function(bob)

 print ("["..bob:Name().."]: ".."Goin' to the bank. Yes siree")

end

VisitBankAndDepositGold["Execute"] = function(bob)

 bob:AddToWealth(bob:GoldCarried())
 bob:SetGoldCarried(0)

 print ("["..bob:Name().."]: ".."Depositing gold. Total savings now: "..bob:Wealth())

  if bob:Wealth() >= bob.ComfortLevel then
   bob:GetFSM():ChangeState(GoHomeAndSleepTilRested)
 else
   bob:GetFSM():ChangeState(EnterMineAndDigForNugget)
 end
end

VisitBankAndDepositGold["Exit"] = function(bob)

  print ("["..bob:Name().."]: ".."Leavin' the bank")

end



GoHomeAndSleepTilRested = {}

GoHomeAndSleepTilRested["Enter"] = function(bob)

 if eat == false then
  print ("["..bob:Name().."]: ".."Walkin' home")
  print ("["..bob:Name().."]: ".."Hi Honey! I'm home")
  MessageDispatcher.Instance():DispatchMessage(0, bob:ID(), 1, 0)
 end
end

GoHomeAndSleepTilRested["Execute"] = function(bob)

 if bob:Fatigued() then
  bob:DecreaseFatigue()
  print ("["..bob:Name().."]: ".."ZZZZ...")
 else 
  print ("["..bob:Name().."]: ".."What a God darn fantastic nap! Time to find more gold")
  eat = false
  bob:GetFSM():ChangeState(EnterMineAndDigForNugget)
 end
end

GoHomeAndSleepTilRested["Exit"] = function(bob)

end


GoHomeAndSleepTilRested["OnMessage"] = function(bob, msg)

 if msg.Msg == 1 then
  print ("["..bob:Name().."]: ".."Okay Hun, ahm a comin'!")
  bob:GetFSM():ChangeState(EatStew)
 end
end




EatStew = {}

EatStew["Enter"] = function(bob)

 print ("["..bob:Name().."]: ".."Smells Reaaal goood Elsa!")

end

EatStew["Execute"] = function(bob)

  print ("["..bob:Name().."]: ".."Tastes real good too!")
  eat = true
  bob:GetFSM():RevertToPreviousState()

end

EatStew["Exit"] = function(bob)

   print ("["..bob:Name().."]: ".."Thankya li'lle lady. Ah better get back to whatever ah wuz doin'")

end






