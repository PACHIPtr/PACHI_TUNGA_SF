import uiScriptLocale

RUMI_ROOT = 'd:/ymir work/ui/'
window = {
	'name': 'EventDialog',
	"x" : SCREEN_WIDTH - 136 - 100-10,
	"y" : 15,
	'width': 100,
	'height': 58,
	'children':
	(
		{
			'name': 'event_window',
			'type': 'window',
			'x': 0,
			'y': 0,
			'width': 100,
			'height': 58,
			'children':
			(
				{
					'name': 'game_event_button',
					'type': 'button',
					'x': 0,
					'y': 0,
					'default_image': RUMI_ROOT + 'event_icon.tga',
					'over_image': RUMI_ROOT + 'event_icon.tga',
					'down_image': RUMI_ROOT + 'event_icon.tga'
				},
			)
		},
	)
}