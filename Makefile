# Makefile for OM-RND16 VCV Rack Plugin

RACK_DIR ?= ../Rack-SDK
SOURCES = $(wildcard src/*.cpp)

include $(RACK_DIR)/plugin.mk
