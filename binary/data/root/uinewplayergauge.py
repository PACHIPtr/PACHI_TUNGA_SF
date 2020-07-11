import chr
import player
import textTail
import ui

class NewPlayerGauge(ui.Gauge):

	def __init__(self, parent):
		ui.Gauge.__init__(self)
		self.SetParent(parent)
		self.AddFlag('not_pick')
		self.MakeGauge(100, 'red')
		self.vid = 0
		self.hpPercentage = 0
		self.curGauge = 0.0

	def __del__(self):
		ui.Gauge.__del__(self)

	def Hide(self):
		self.vid = 0
		self.curGauge = 0.0
		self.hpPercentage = 0
		ui.Gauge.Hide(self)

	def OnUpdate(self):
		distance = player.GetCharacterDistance(self.vid)
		if distance > 1500:
			self.Hide()
			return

		if chr.GetNameByVID(self.vid) == 'None':
			self.Hide()
			return

		if self.hpPercentage <= 0:
			self.Hide()
			return

		self.destGauge = float(self.hpPercentage)
		self.curGauge += (self.destGauge - self.curGauge) / 10.0

		if abs(self.curGauge - self.destGauge) < 0.005:
			self.curGauge = self.destGauge

		self.SetPercentage(self.curGauge, 100)
		x, y, z = textTail.GetPosition(self.vid)
		isChat = textTail.IsChat(self.vid)
		ui.Gauge.SetPosition(self, int(x - self.GetWidth() / 2), int(y + 5) + isChat * 17)

	def RefreshGauge(self, vid, hpPercentage):
		self.vid = vid
		self.hpPercentage = hpPercentage

		if hpPercentage == 100:
			self.Hide()
		else:
			self.OnUpdate()
			self.Show()
