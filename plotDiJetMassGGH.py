#!/usr/bin/env python

import glob
import ROOT as root
from src.helpers import *

root.gROOT.SetBatch(True)

ggH = root.TFile("ggHmumu_13TeV_Hist.root")

canvas = root.TCanvas()

#Set name is for the stat boxes
gghHist = ggH.Get("diJetMassHist")
gghHist.SetName("ggHmumu")

xMin = 0
xMax = 200
yMin = 0
yMax = 3000
xTitle = "2 Jet Mass [GeV/c^{2}]"

axisHist = root.TH2F("axisHist","DiJet Mass",1,xMin,xMax,1,yMin,yMax)
axisHist.GetXaxis().SetTitle(xTitle)
axisHist.GetYaxis().SetTitle("Events/2 GeV")
axisHist.GetYaxis().SetTitleOffset(1.7)
axisHist.Draw()

#root.gStyle.SetOptStat(1111)

gghHist.SetLineColor(4)
gghHist.SetMarkerStyle(0)
gghHist.SetLineStyle(1)

leg = root.TLegend(.7,.7,.9,.9,"MC sample")
leg.AddEntry(gghHist,"ggHmumu","l")

#canvas.GetPad(0).Update()
#stats_vhmumu = vhmumuHist.GetListOfFunctions().FindObject("stats").Clone("stats_vhmumu")

#y1 = stats_vhmumu.GetY1NDC()
#y2 = stats_vhmumu.GetY2NDC()

#stats_vhmumu.SetY1NDC(2 * y1 - y2)
#stats_vhmumu.SetY2NDC(y1)
#stats_vhmumu.Draw()
gghHist.Draw("SAMES")
leg.Draw()

canvas.SaveAs("Hist_diJetMassGGH.png")

