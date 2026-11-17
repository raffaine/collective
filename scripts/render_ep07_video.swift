import Foundation
import AVFoundation
import CoreGraphics
import CoreText
import AppKit

struct SceneSpec {
    let path: String
    let actLabel: String
    let placeLabel: String
}

// 30 unique eye-level walking scenes across Episode 07 (Wave D: Eta, Mu, Theta, Nu, Xi)
let scenes: [SceneSpec] = [
    // Part I: Scenario Eta — Regenerative Agroforestry, Rotational Coppice, Mother-Tree Vetoes & Forest Apothecary
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_01.png", actLabel: "PART I // SCENARIO ETA: WINTER SAP DORMANCY", placeLabel: "Rotational Black-Locust & Chestnut Coppice Grove"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_02.png", actLabel: "PART I // SCENARIO ETA: MOTHER-TREE SANCTUARY VETO", placeLabel: "Inviolable Old-Growth Douglas Fir & Willow Fence"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_03.png", actLabel: "PART I // SCENARIO ETA: <=25% FORAGING CEILING", placeLabel: "1-in-4 Ethical Wild-Foraging & Fern Brook Trail"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_03.png", actLabel: "PART I // SCENARIO ETA: 94% DORMANT WORKFLOWS", placeLabel: "Solar Dehumidification Lumber Kiln (<=1200 bp)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_09.png", actLabel: "PART I // SCENARIO ETA: DUAL-ASSAY APOTHECARY", placeLabel: "0 ng Amatoxin & <100 ppb Botanical Wet-Lab"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_02.png", actLabel: "PART I // SCENARIO ETA: 70-90% FOREST TAX SLASH", placeLabel: "Conservation Easement Seven-Strata Food Forest"),

    // Part II: Scenario Mu — Five-Stream Urban Unbuilding, 72h Compost/Struvite & Tipping-Fee Inversion
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_04.png", actLabel: "PART II // SCENARIO MU: >=98% LANDFILL DIVERSION", placeLabel: "Five-Stream Surgical Unbuilding & NIR/XRF Yard"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_11.png", actLabel: "PART II // SCENARIO MU: TIPPING-FEE INVERSION", placeLabel: "Reclaimed Old-Growth Beams & Copper Busbar Crib"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_05.png", actLabel: "PART II // SCENARIO MU: 72H THERMOPHILIC RESET", placeLabel: "55°C Class-A Compost & Smokeless TLUD Biochar"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_15.png", actLabel: "PART II // SCENARIO MU: >=90% STRUVITE RECOVERY", placeLabel: "Closed-Loop Phosphorus & Nitrogen Nutrient Cycle"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_01.png", actLabel: "PART II // SCENARIO MU: LIVING TERRA PRETA", placeLabel: "Biochar-Charged Keyline Market Garden Soils"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_02.png", actLabel: "PART II // SCENARIO MU: CRYPTOGRAPHIC TAINT LOCK", placeLabel: "Phytomining Quarantine & Zero-Dumping Diode"),

    // Part III: Scenario Theta — Three-Tier Material Hierarchy, Solar Crucible Batching & <=250ms Pour Gates
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_07.png", actLabel: "PART III // SCENARIO THETA: TIER-1 SALVAGE FIRST", placeLabel: "Direct Dimensional Reuse (<5% Remelt Exergy)"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_06.png", actLabel: "PART III // SCENARIO THETA: <=250MS 2-PERSON POUR", placeLabel: "Solar-Batched Induction Crucible & Dead-Man Gates"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_08.png", actLabel: "PART III // SCENARIO THETA: XRF ALLOY METROLOGY", placeLabel: "Zero-Downcycling Silicon Bronze & 316L Ingots"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_07.png", actLabel: "PART III // SCENARIO THETA: SILICATE GLASS TANKS", placeLabel: "Recycled Canning Jars & Borosilicate Labware Kiln"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_14.png", actLabel: "PART III // SCENARIO THETA: CASCADE EXERGY REUSE", placeLabel: "Foundry Cooling-Jacket Heat to District Hydronics"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_08.png", actLabel: "PART III // SCENARIO THETA: CHLOR-ALKALI REAGENTS", placeLabel: "Solar Brine Electrolysis for Soap Lye & Lime"),

    // Part IV: Scenario Nu — Bioregional Fiber Shed, Zero-PFAS Vats & OP_PURGE_R0_LINEAGE Tailoring
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_08.png", actLabel: "PART IV // SCENARIO NU: BIOREGIONAL FIBER SHED", placeLabel: "Blooming Flax Linen, Hemp & Millstream Scutching"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_02.png", actLabel: "PART IV // SCENARIO NU: ROTATIONAL SHEAR WOOL", placeLabel: "Silvopasture Meadow & Oak-Bark Vegetable Tannin"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_09.png", actLabel: "PART IV // SCENARIO NU: ZERO-PFAS BOTANICAL VATS", placeLabel: "Indigo, Madder & Golden Weld Natural Dye Courtyard"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_10.png", actLabel: "PART IV // SCENARIO NU: OP_PURGE_R0_LINEAGE", placeLabel: "Ephemeral-RAM Cobbler Lasts & Goodyear-Welt Boots"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_09.png", actLabel: "PART IV // SCENARIO NU: SASHIKO VISIBLE MENDING", placeLabel: "Neighborhood Sewing Loft (<15% New Exergy)"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_14.png", actLabel: "PART IV // SCENARIO NU: ZERO UNIFORM MANDATES", placeLabel: "Vibrant Bespoke Attire & Courtyard Gathering"),

    // Part V: Scenario Xi — Habraken Two-Track Open Building, Reversible Kigumi Joinery & Barn-Raisings
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_11.png", actLabel: "PART V // SCENARIO XI: FENCE UNIFICATION", placeLabel: "Opened Suburban Backyard Park & Solar Pergola"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_12.png", actLabel: "PART V // SCENARIO XI: REVERSIBLE CNC KIGUMI", placeLabel: "Nail-Free Mortise-and-Tenon & Oak Drawbore Pegs"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_13.png", actLabel: "PART V // SCENARIO XI: TRACK-1 SUPPORT (2.5X FEA)", placeLabel: "Weekend Community Barn-Raising & Hemp-Lime Walls"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_14.png", actLabel: "PART V // SCENARIO XI: TRACK-2 SOVEREIGN INFILL", placeLabel: "Zero-Telemetry Demountable Interior Partitions"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_11.png", actLabel: "PART V // SCENARIO XI: PE STAMP CODE ARMOR", placeLabel: "IBC 104.11 Alternative-Material Timber Courtyard"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_15.png", actLabel: "PART V // WAVE D SYNTHESIS: CIRCULAR BIOREGION", placeLabel: "Golden-Twilight Timber Village & Coppice Hills")
]

let width = 1920
let height = 1080
let fps: Int32 = 8
let crossfadeSec: Double = 5.0
let colorSpace = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue

func loadNormalizedCGImage(_ path: String) -> CGImage {
    guard let nsImg = NSImage(contentsOfFile: path),
          let srcCG = nsImg.cgImage(forProposedRect: nil, context: nil, hints: nil) else {
        fatalError("Failed to load image at \(path)")
    }
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create normalize CGContext")
    }
    ctx.interpolationQuality = .high
    let scaleX = CGFloat(width) / CGFloat(srcCG.width)
    let scaleY = CGFloat(height) / CGFloat(srcCG.height)
    let scale = max(scaleX, scaleY)
    let drawW = CGFloat(srcCG.width) * scale
    let drawH = CGFloat(srcCG.height) * scale
    let dx = (CGFloat(width) - drawW) * 0.5
    let dy = (CGFloat(height) - drawH) * 0.5
    ctx.draw(srcCG, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    return ctx.makeImage()!
}

func makeHudCGImage(actLabel: String, placeLabel: String) -> CGImage {
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create HUD CGContext")
    }

    let boxRect = CGRect(x: 48, y: 42, width: 760, height: 86)
    let boxPath = CGPath(roundedRect: boxRect, cornerWidth: 8, cornerHeight: 8, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.62))
    ctx.addPath(boxPath)
    ctx.fillPath()

    ctx.setStrokeColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.45))
    ctx.setLineWidth(1.5)
    ctx.addPath(boxPath)
    ctx.strokePath()

    let pillRect = CGRect(x: 48, y: height - 66, width: 660, height: 34)
    let pillPath = CGPath(roundedRect: pillRect, cornerWidth: 6, cornerHeight: 6, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.55))
    ctx.addPath(pillPath)
    ctx.fillPath()

    func drawText(_ text: String, font: CTFont, color: CGColor, x: CGFloat, y: CGFloat) {
        let attrs: [NSAttributedString.Key: Any] = [
            NSAttributedString.Key(kCTFontAttributeName as String): font,
            NSAttributedString.Key(kCTForegroundColorAttributeName as String): color
        ]
        let attrStr = NSAttributedString(string: text, attributes: attrs)
        let line = CTLineCreateWithAttributedString(attrStr)
        ctx.textPosition = CGPoint(x: x, y: y)
        CTLineDraw(line, ctx)
    }

    let fontTop = CTFontCreateWithName("Courier-Bold" as CFString, 15, nil)
    let fontAct = CTFontCreateWithName("Courier-Bold" as CFString, 16, nil)
    let fontPlace = CTFontCreateWithName("Helvetica-Bold" as CFString, 23, nil)

    let gold = CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.95)
    let green = CGColor(red: 0.48, green: 0.82, blue: 0.42, alpha: 0.95)
    let white = CGColor(red: 0.95, green: 0.96, blue: 0.98, alpha: 0.96)

    drawText("RADIO LAMBDA // EP. 07: WAVE D — ETA, MU, THETA, NU & XI", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/episodes/07.episode.m4a"
        let finalOutPath = "doc/podcast/episodes/07.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ep07_silent_video.mp4"

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        try? FileManager.default.removeItem(atPath: finalOutPath)

        let audioURL = URL(fileURLWithPath: audioPath)
        let audioAsset = AVURLAsset(url: audioURL)
        let durationCM = try await audioAsset.load(.duration)
        let durationSec = CMTimeGetSeconds(durationCM)
        print(String(format: "Audio duration: %.2f seconds (%.2f minutes)", durationSec, durationSec / 60.0))

        let cgScenes = scenes.map { loadNormalizedCGImage($0.path) }
        let hudScenes = scenes.map { makeHudCGImage(actLabel: $0.actLabel, placeLabel: $0.placeLabel) }
        let sceneDuration = durationSec / Double(scenes.count)

        let writer = try AVAssetWriter(outputURL: URL(fileURLWithPath: tempVideoPath), fileType: .mp4)
        let videoSettings: [String: Any] = [
            AVVideoCodecKey: AVVideoCodecType.h264,
            AVVideoWidthKey: width,
            AVVideoHeightKey: height,
            AVVideoCompressionPropertiesKey: [
                AVVideoAverageBitRateKey: 1_800_000,
                AVVideoMaxKeyFrameIntervalKey: 16,
                AVVideoProfileLevelKey: AVVideoProfileLevelH264HighAutoLevel
            ]
        ]
        let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
        writerInput.expectsMediaDataInRealTime = false

        let pixelBufferAttrs: [String: Any] = [
            kCVPixelBufferPixelFormatTypeKey as String: Int(kCVPixelFormatType_32BGRA),
            kCVPixelBufferWidthKey as String: width,
            kCVPixelBufferHeightKey as String: height,
            kCVPixelBufferCGImageCompatibilityKey as String: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey as String: true
        ]
        let adaptor = AVAssetWriterInputPixelBufferAdaptor(
            assetWriterInput: writerInput,
            sourcePixelBufferAttributes: pixelBufferAttrs
        )

        writer.add(writerInput)
        writer.startWriting()
        writer.startSession(atSourceTime: .zero)

        let totalFrames = Int(ceil(durationSec * Double(fps)))
        print("Rendering \(totalFrames) frames at \(fps) fps across \(scenes.count) scenes...")

        func drawSceneKenBurns(_ ctx: CGContext, img: CGImage, index: Int, progress: Double, alpha: CGFloat) {
            let p = CGFloat(max(0.0, min(1.0, progress)))
            let zoomIn = (index % 2 == 0)
            let scale: CGFloat = zoomIn ? (1.02 + 0.08 * p) : (1.10 - 0.08 * p)
            let panDir: CGFloat = (index % 3 == 0) ? 1.0 : ((index % 3 == 1) ? -1.0 : 0.4)
            let maxPanX = CGFloat(width) * 0.025
            let maxPanY = CGFloat(height) * 0.015
            let offsetX = panDir * maxPanX * (p - 0.5)
            let offsetY = -panDir * maxPanY * (p - 0.5)

            let drawW = CGFloat(width) * scale
            let drawH = CGFloat(height) * scale
            let x = (CGFloat(width) - drawW) * 0.5 + offsetX
            let y = (CGFloat(height) - drawH) * 0.5 + offsetY

            ctx.saveGState()
            if alpha < 0.999 {
                ctx.setAlpha(alpha)
            }
            ctx.interpolationQuality = .low
            ctx.draw(img, in: CGRect(x: x, y: y, width: drawW, height: drawH))
            ctx.restoreGState()
        }

        for frameIdx in 0..<totalFrames {
            while !writerInput.isReadyForMoreMediaData {
                Thread.sleep(forTimeInterval: 0.002)
            }

            autoreleasepool {
                let t = Double(frameIdx) / Double(fps)
                let rawIdx = Int(t / sceneDuration)
                let curIdx = min( max(0, rawIdx), scenes.count - 1 )
                let sceneLocalT = t - Double(curIdx) * sceneDuration
                let prog = sceneLocalT / sceneDuration

                var pixelBufferOut: CVPixelBuffer?
                CVPixelBufferPoolCreatePixelBuffer(nil, adaptor.pixelBufferPool!, &pixelBufferOut)
                guard let pixelBuffer = pixelBufferOut else { return }

                CVPixelBufferLockBaseAddress(pixelBuffer, [])
                let baseAddr = CVPixelBufferGetBaseAddress(pixelBuffer)!
                let bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer)

                let ctx = CGContext(
                    data: baseAddr,
                    width: width,
                    height: height,
                    bitsPerComponent: 8,
                    bytesPerRow: bytesPerRow,
                    space: colorSpace,
                    bitmapInfo: bitmapInfo
                )!

                drawSceneKenBurns(ctx, img: cgScenes[curIdx], index: curIdx, progress: prog, alpha: 1.0)

                if curIdx + 1 < scenes.count && (sceneDuration - sceneLocalT) < crossfadeSec {
                    let fadeAlpha = CGFloat((crossfadeSec - (sceneDuration - sceneLocalT)) / crossfadeSec)
                    drawSceneKenBurns(ctx, img: cgScenes[curIdx + 1], index: curIdx + 1, progress: 0.0, alpha: fadeAlpha)
                }

                ctx.interpolationQuality = .none
                ctx.draw(hudScenes[curIdx], in: CGRect(x: 0, y: 0, width: width, height: height))

                let totalProg = CGFloat(t / durationSec)
                ctx.setFillColor(CGColor(red: 1.0, green: 1.0, blue: 1.0, alpha: 0.18))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width), height: 6))
                ctx.setFillColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.92))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width) * totalProg, height: 6))

                CVPixelBufferUnlockBaseAddress(pixelBuffer, [])

                let pts = CMTime(value: CMTimeValue(frameIdx), timescale: fps)
                adaptor.append(pixelBuffer, withPresentationTime: pts)
            }

            if frameIdx % 1920 == 0 && frameIdx > 0 {
                let pct = Int((Double(frameIdx) / Double(totalFrames)) * 100.0)
                print("Progress: \(pct)% (\(frameIdx)/\(totalFrames) frames)")
            }
        }

        writerInput.markAsFinished()
        await writer.finishWriting()
        print("Silent video track rendered. Now muxing audio losslessly...")

        let composition = AVMutableComposition()
        let videoAsset = AVURLAsset(url: URL(fileURLWithPath: tempVideoPath))

        let compVideoTrack = composition.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcVideoTrack = try await videoAsset.loadTracks(withMediaType: .video).first!
        try compVideoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcVideoTrack, at: .zero)

        let compAudioTrack = composition.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcAudioTrack = try await audioAsset.loadTracks(withMediaType: .audio).first!
        try compAudioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcAudioTrack, at: .zero)

        let exporter = AVAssetExportSession(asset: composition, presetName: AVAssetExportPresetPassthrough)!
        exporter.outputURL = URL(fileURLWithPath: finalOutPath)
        exporter.outputFileType = .mp4
        await exporter.export()

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        print("SUCCESS: Created YouTube video at \(finalOutPath)")
        semaphore.signal()
    } catch {
        print("ERROR: \(error)")
        exit(1)
    }
}

semaphore.wait()
