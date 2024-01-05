extends Label

@export var bus_name: String = "Master"
@export var label: String = "Sound"
@export var label_font_size: int = 20
var bus_id: int

func _ready() -> void:
	bus_id = AudioServer.get_bus_index(bus_name)
	$VolumeSlider.connect("value_changed", _on_value_changed)
	$VolumeSlider.value = db_to_linear(AudioServer.get_bus_volume_db(bus_id))
	$Text.text = label
	$Text.label_settings.font_size = label_font_size

func _on_value_changed(val: float) -> void:
	AudioServer.set_bus_volume_db(bus_id, linear_to_db(val))
