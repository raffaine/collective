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

// 30 unique eye-level walking scenes across Episode 08 (Wave E: Epsilon, Iota, Lambda, Pi)
let scenes: [SceneSpec] = [
    // Part I: Scenario Epsilon — Shared Third-Place Portal, Dual-VLAN Coworking & Carapace-as-a-Service
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_01.png", actLabel: "PART I // SCENARIO EPSILON: ZERO-ID NEIGHBOR PORTAL", placeLabel: "Street-Facing Third-Place Cafe & Matte Kiosk"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_05.png", actLabel: "PART I // SCENARIO EPSILON: CIVIC HOSPITALITY", placeLabel: "Town-Hall Arcade & Public Environmental Board"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_02.png", actLabel: "PART I // SCENARIO EPSILON: HARDWARE VLAN 40/30", placeLabel: "40 dBA Study Hall & Brass Rotary VLAN Selector"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_02.png", actLabel: "PART I // SCENARIO EPSILON: SPYWARE QUARANTINE", placeLabel: "Outbound DMZ Diode & Sovereign Mesh Air-Gap"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_03.png", actLabel: "PART I // SCENARIO EPSILON: >=80% COMPUTE HEAT", placeLabel: "Warm-Water Server Manifolds to Winter Greenhouse"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_04.png", actLabel: "PART I // SCENARIO EPSILON: HYDRONIC CO-DESIGN", placeLabel: "Bathhouse & Café Radiant Floor Thermal Loop"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_12.png", actLabel: "PART I // SCENARIO EPSILON: CARAPACE-AS-A-SERVICE", placeLabel: "Subchapter T Fiduciary Studio & GPO Pool"),

    // Part II: Scenario Iota — Epistemic Open Science, <=50bp Metrology, Null-Result Parity & Alexandria Bindery
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_04.png", actLabel: "PART II // SCENARIO IOTA: <=50 BP DRIFT GATE", placeLabel: "Vibration-Isolated Metrology & Soil Wet-Lab"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_09.png", actLabel: "PART II // SCENARIO IOTA: PRE-REGISTERED TRIALS", placeLabel: "Botanical Assay Cloister & Reference Standards"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_06.png", actLabel: "PART II // SCENARIO IOTA: N>=3 REPLICATION", placeLabel: "Three-Valley Botanical Observatory & Weather Dome"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_01.png", actLabel: "PART II // SCENARIO IOTA: NULL-RESULT PARITY", placeLabel: "Replicated Keyline Cultivar Field Trial Beds"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_05.png", actLabel: "PART II // SCENARIO IOTA: ALEXANDRIA BINDERY", placeLabel: "Offline IPFS Library & Archival Rag-Paper Press"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_02.png", actLabel: "PART II // SCENARIO IOTA: SOCRATIC INQUIRY", placeLabel: "Sunlit Timber Reading Room & Multi-Age Circle"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_10.png", actLabel: "PART II // SCENARIO IOTA: DEFENSIVE PRIOR ART", placeLabel: "Lakeside Alexandria Archive & Open Schematics"),

    // Part III: Scenario Lambda — Unmediated Performance, CITES-Free Luthiery & Perimeter SPL Proofs
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_07.png", actLabel: "PART III // SCENARIO LAMBDA: UNMEDIATED LATCH", placeLabel: "Music Parlor & Open-Circuit Brass Mic Kill-Switch"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_09.png", actLabel: "PART III // SCENARIO LAMBDA: <=800 BP TONEWOOD", placeLabel: "CITES-Free Maple & Black-Locust Luthiery Bay"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_08.png", actLabel: "PART III // SCENARIO LAMBDA: INSTRUMENT COMMONS", placeLabel: "Seasoned Violin Backs & Cast Silicon-Bronze Bells"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_09.png", actLabel: "PART III // SCENARIO LAMBDA: ZERO-FEED BROADSIDES", placeLabel: "Letterpress Printshop & Earth-Pigment Studio"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_03.png", actLabel: "PART III // SCENARIO LAMBDA: PHENOLOGICAL FEASTS", placeLabel: "Stone Amphitheater & Solstice Gathering Lawn"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_10.png", actLabel: "PART III // SCENARIO LAMBDA: PERIMETER SPL LANTERN", placeLabel: "Woodland Stage & Warm-Amber Decibel Lantern"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_14.png", actLabel: "PART III // SCENARIO LAMBDA: ACOUSTIC COURTESY", placeLabel: "Lantern-Lit Courtyard Table & Unrecorded Song"),

    // Part IV: Scenario Pi — Polycentric Guild Patchwork, Auto-Dissolving Swarms & MeshGrowthBeacons
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_11.png", actLabel: "PART IV // SCENARIO PI: 3D POLYCENTRIC PATCHWORK", placeLabel: "Watershed Relief Map & Bicameral Council Rotunda"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_13.png", actLabel: "PART IV // SCENARIO PI: EPHEMERAL WORK-BEE SWARM", placeLabel: "Cross-Guild Barn-Raising Task Force in Action"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_12.png", actLabel: "PART IV // SCENARIO PI: AUTO-DISSOLVE ON RECEIPT", placeLabel: "Coiled Ropes, Tool Chests & Completed Swarm Bench"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_13.png", actLabel: "PART IV // SCENARIO PI: 4-STAGE RICARDIAN TREATY", placeLabel: "Inter-Valley Stone Footbridge & Sister Villages"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_14.png", actLabel: "PART IV // SCENARIO PI: ZERO-SUM PAIRWISE BCC", placeLabel: "Canal Wharf Exchange & Solar Freight Barge"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_12.png", actLabel: "PART IV // SCENARIO PI: MESH GROWTH BEACON", placeLabel: "75% Haul-Exergy Reduction & Fellowship Route"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_09.png", actLabel: "PART IV // SCENARIO PI: ANTI-ISOLATIONISM GUARD", placeLabel: "Shared Valley Canal & Reciprocal Headroom"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_15.png", actLabel: "PART IV // WAVE E SYNTHESIS: LIVING COMMONWEALTH", placeLabel: "Twilight Federated Watershed & Dark-Sky Villages")
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

    drawText("RADIO LAMBDA // EP. 08: WAVE E — EPSILON, IOTA, LAMBDA & PI", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/episodes/08.episode.m4a"
        let finalOutPath = "doc/podcast/episodes/08.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ep08_silent_video.mp4"

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
