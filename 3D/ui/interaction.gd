extends Control

@export var key: Enums.INTERACTION_KEY = Enums.INTERACTION_KEY.E
@export var text: String = "Interact"
@export var time: int = 0

func _ready():
	init(key, text)

func _process(_delta):
	pass

#-----------------------------------------------

func load_texture() -> void:
	match key:
		Enums.INTERACTION_KEY.E:
			$icon.texture = load("res://art/ui/hud/UI_Keyboard_Icon_E.png")
		Enums.INTERACTION_KEY.R:
			$icon.texture = load("res://art/ui/hud/UI_Keyboard_Icon_R.png")
		Enums.INTERACTION_KEY.TAB:
			$icon.texture = load("res://art/ui/hud/UI_Keyboard_Icon_Tab.png")
		Enums.INTERACTION_KEY.HEALTH:
			$icon.texture = load("res://art/ui/drops/HealthDrop.png")
		Enums.INTERACTION_KEY.AMMO:
			$icon.texture = load("res://art/ui/hud/ammo_icon.png")
		Enums.INTERACTION_KEY.NONE:
			pass

func set_key(new_key: Enums.INTERACTION_KEY):
	key = new_key
	if key != Enums.INTERACTION_KEY.NONE:
		load_texture()

func set_text(new_text: String):
	text = new_text
	$text.text = text

func init(new_key: Enums.INTERACTION_KEY, new_text: String):
	set_key(new_key)
	set_text(new_text)
	hide_interaction()

func show_interaction():
	self.show()
	if time == 0:
		return
	
	$timer.stop()
	$timer.start(time)
	await $timer.timeout
	hide_interaction()

func hide_interaction():
	$timer.stop()
	self.hide()

