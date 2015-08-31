#!/usr/bin/env python

import glob
import ROOT as root
from src.helpers import *

root.gROOT.SetBatch(True)

vhmumu = root.TFile("VHmumu_13TeV_Hist.root")
tt = root.TFile("TT_13TeV_Hist.root")

canvas = root.TCanvas()

vhmumuHist = vhmumu.Get("diJetMassHist")
ttHist = tt.Get("diJetMassHist")

xMin = 0
xMax = 200
yMin = 0
yMax = 4000
xTitle = "2 Jet Mass [GeV/c^{2}]"

axisHist = root.TH2F("axisHist","",1,xMin,xMax,1,yMin,yMax)
axisHist.GetXaxis().SetTitle(xTitle)
axisHist.GetYaxis().SetTitle("Events/Bin")
axisHist.Draw()

vhmumuHist.SetLineColor(1)
vhmumuHist.SetMarkerStyle(0)
vhmumuHist.SetLineStyle(1)
ttHist.SetLineColor(2)
ttHist.SetMarkerStyle(0)
ttHist.SetLineStyle(1)

leg = root.TLegend(.2,.7,.4,.9,"MC sample")
leg.AddEntry(vhmumuHist,"vhmumu","l")
leg.AddEntry(ttHist,"TT","l")

vhmumuHist.Draw("HIST same")
ttHist.Draw("HIST same")
leg.Draw()

canvas.SaveAs("Hist_diJetMass.png")
