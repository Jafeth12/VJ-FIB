extends Control

enum INTERACTION_KEY {
	E,
	TAB,
	NONE
}

@export var key: INTERACTION_KEY = INTERACTION_KEY.E
@export var text: String = "Interact"

var sprites = [
	Image.load_from_file("res://art/ui/hud/UI_Keyboard_Icon_E.png"),
	# Image.load_from_file("res://art/ui/hud/UI_Keyboard_Icon_Tab.png")
]

func _ready():
	init(key, text)

func _process(_delta):
	pass

#-----------------------------------------------

func set_key(new_key: INTERACTION_KEY):
	key = new_key
	if key == INTERACTION_KEY.NONE:
		$icon.hide()
	else:
		$icon.texture = ImageTexture.create_from_image(sprites[(int)(key)])
		$icon.show()

func set_text(new_text: String):
	text = new_text
	$text.text = text

func init(new_key: INTERACTION_KEY, new_text: String):
	set_key(new_key)
	set_text(new_text)
	hide_interaction()

func show_interaction():
	self.show()

func hide_interaction():
	self.hide()

