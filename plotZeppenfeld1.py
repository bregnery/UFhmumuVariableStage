#!/usr/bin/env python

import glob
import ROOT as root
from src.helpers import *

root.gROOT.SetBatch(True)

vhmumu = root.TFile("VHmumu_13TeV_Hist.root")
vbf = root.TFile("VBFHmumu_13TeV_Hist.root")
ggH = root.TFile("ggHmumu_13TeV_Hist.root")
tt = root.TFile("TT_13TeV_Hist.root")
dy = root.TFile("DY_13TeV_Hist.root")
ttH = root.TFile("ttHmumu_13TeV_Hist.root")

canvas = root.TCanvas()

#Set name is for the stat boxes
vhmumuHist = vhmumu.Get("Zeppenfeld1Hist")
vhmumuHist.SetName("vHmumu")
ttHist = tt.Get("Zeppenfeld1Hist")
ttHist.SetName("TT")

vbfHist = vbf.Get("Zeppenfeld1Hist")
gghHist = ggH.Get("Zeppenfeld1Hist")
dyHist = dy.Get("Zeppenfeld1Hist")
tthHist = ttH.Get("Zeppenfeld1Hist")

xMin = -6
xMax = 6
yMin = 0
yMax = 25000
xTitle = "#eta_{1} *"

axisHist = root.TH2F("axisHist","Zeppenfeld 1",1,xMin,xMax,1,yMin,yMax)
axisHist.GetXaxis().SetTitle(xTitle)
axisHist.GetYaxis().SetTitle("Events")
axisHist.GetYaxis().SetTitleOffset(1.7)
axisHist.Draw()

#root.gStyle.SetOptStat(1111)

vhmumuHist.SetLineColor(1)
vhmumuHist.SetMarkerStyle(0)
vhmumuHist.SetLineStyle(1)
ttHist.SetLineColor(2)
ttHist.SetMarkerStyle(0)
ttHist.SetLineStyle(1)
vbfHist.SetLineColor(3)
vbfHist.SetMarkerStyle(0)
vbfHist.SetLineStyle(1)
gghHist.SetLineColor(4)
gghHist.SetMarkerStyle(0)
gghHist.SetLineStyle(1)
dyHist.SetLineColor(52)
dyHist.SetMarkerStyle(0)
dyHist.SetLineStyle(1)
tthHist.SetLineColor(7)
tthHist.SetMarkerStyle(0)
tthHist.SetLineStyle(1)

leg = root.TLegend(.7,.7,.9,.9,"MC sample")
leg.AddEntry(vhmumuHist,"vhmumu","l")
leg.AddEntry(ttHist,"TT","l")
leg.AddEntry(vbfHist,"vbfHmumu","l")
leg.AddEntry(gghHist,"ggHmumu","l")
leg.AddEntry(dyHist,"DY","l")
leg.AddEntry(tthHist,"ttHmumu","l")

vhmumuHist.Draw("Hist SAME")
#canvas.GetPad(0).Update()
#stats_vhmumu = vhmumuHist.GetListOfFunctions().FindObject("stats").Clone("stats_vhmumu")

#y1 = stats_vhmumu.GetY1NDC()
#y2 = stats_vhmumu.GetY2NDC()

#stats_vhmumu.SetY1NDC(2 * y1 - y2)
#stats_vhmumu.SetY2NDC(y1)
#stats_vhmumu.Draw()
ttHist.Draw("Hist SAME")
vbfHist.Draw("Hist SAME")
gghHist.Draw("Hist SAME")
dyHist.Draw("Hist SAME")
tthHist.Draw("Hist SAME")
leg.Draw()

canvas.SaveAs("Hist_Zeppenfeld1.png")

