#!/usr/bin/env python

import glob
import ROOT as root
from src.helpers import *

root.gROOT.SetBatch(True)

# Open root files
vhmumu = root.TFile("VHmumu_13TeV_Hist.root")
tt = root.TFile("TT_13TeV_Hist.root")

# Create TCanvas
canvas = root.TCanvas()

# Access Histograms
vhmumuHist = vhmumu.Get("phiStarHist")
vhmumuHist.SetName("vHmumu")
ttHist = tt.Get("phiStarHist")
ttHist.SetName("TT")

# Create a 2D Histogram for more control
xMin = 0
xMax = 1
yMin = 0
yMax = 10000
xTitle = "#phi *"

axisHist = root.TH2F("axisHist","",1,xMin,xMax,1,yMin,yMax)
axisHist.GetXaxis().SetTitle(xTitle)
axisHist.GetYaxis().SetTitle("Events/Bin")
axisHist.Draw()

root.gStyle.SetOptStat(1111)

# Set Histogram settings
vhmumuHist.SetLineColor(1)
vhmumuHist.SetMarkerStyle(0)
vhmumuHist.SetLineStyle(1)
ttHist.SetLineColor(2)
ttHist.SetMarkerStyle(0)
ttHist.SetLineStyle(1)

# Create a legend
leg = root.TLegend(.2,.7,.4,.9,"MC sample")
leg.AddEntry(vhmumuHist,"vhmumu","l")
leg.AddEntry(ttHist,"TT","l")

# Draw the histogram
vhmumuHist.Draw("SAMES")
canvas.GetPad(0).Update()
stats_vhmumu = vhmumuHist.GetListOfFunctions().FindObject("stats").Clone("stats_vhmumu")

y1 = stats_vhmumu.GetY1NDC()
y2 = stats_vhmumu.GetY2NDC()

stats_vhmumu.SetY1NDC(2 * y1 - y2)
stats_vhmumu.SetY2NDC(y1)
stats_vhmumu.Draw()
ttHist.Draw("SAMES")
#stats_tt = ttHist.GetListOfFunctions().FindObject("stats").Clone("stats_tt")
#stats_tt.SetY1NDC(.1)
#stats_tt.SetY2NDC(.3)
#stats_tt.Draw()
leg.Draw()
canvas.SaveAs("Hist_phiStar.png")

