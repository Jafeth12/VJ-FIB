extends Control

var selected_weapon = null

# Called when the node enters the scene tree for the first time.
func _ready():
	set_health(100)
	select_pistol()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

# ------------------------------

func set_health(health: int) -> void:
	$health_bar.value = health
	$health_label.text = str(health)

func set_ammo(ammo: int) -> void:
	selected_weapon.set_ammo(ammo)

func select_pistol() -> void:
	# if selected_weapon == $pistol_hud:
	# 	return
	$rifle_hud.disable()
	$pistol_hud.enable()
	selected_weapon = $pistol_hud

func select_rifle() -> void:
	# if selected_weapon == $rifle_hud:
	# 	return
	$pistol_hud.disable()
	$rifle_hud.enable()
	selected_weapon = $rifle_hud
	
	
