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
vhmumuHist = vhmumu.Get("diJetMassHist")
vhmumuHist.SetName("vHmumu")
ttHist = tt.Get("diJetMassHist")
ttHist.SetName("TT")

vbfHist = vbf.Get("diJetMassHist")
gghHist = ggH.Get("diJetMassHist")
dyHist = dy.Get("diJetMassHist")
tthHist = ttH.Get("diJetMassHist")

xMin = 0
xMax = 200
yMin = 0
yMax = 2100
xTitle = "2 Jet Mass [GeV/c^{2}]"

axisHist = root.TH2F("axisHist","DiJet Mass",1,xMin,xMax,1,yMin,yMax)
axisHist.GetXaxis().SetTitle(xTitle)
axisHist.GetYaxis().SetTitle("Events/2 GeV")
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

vhmumuHist.Draw("SAMES")
#canvas.GetPad(0).Update()
#stats_vhmumu = vhmumuHist.GetListOfFunctions().FindObject("stats").Clone("stats_vhmumu")

#y1 = stats_vhmumu.GetY1NDC()
#y2 = stats_vhmumu.GetY2NDC()

#stats_vhmumu.SetY1NDC(2 * y1 - y2)
#stats_vhmumu.SetY2NDC(y1)
#stats_vhmumu.Draw()
ttHist.Draw("SAMES")
vbfHist.Draw("SAMES")
gghHist.Draw("SAMES")
dyHist.Draw("SAMES")
tthHist.Draw("SAMES")
leg.Draw()

canvas.SaveAs("Hist_diJetMass.png")

